#ifndef MOEXPARSER_REQUEST_TO_MOEX_H
#define MOEXPARSER_REQUEST_TO_MOEX_H


#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/variant.hpp>
#include <boost/property_tree/detail/rapidxml.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "Date.h"
#include "Time.h"
#include "utilities.h"

#if TEST_METHODS || TEST_CLASS
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#define LOG "main.log"
#define TEST_SIZE 100
#endif

#if TEST_METHODS
std::vector<int> t_day;       // 1
std::vector<int> t_target;    // 2
std::vector<int> t_cut;       // 3
std::vector<int> t_substr;    // 4
// std::vector<int> t_split;     // 5
std::vector<int> t_get;       // 6
std::vector<int> t_pars;      // 7
std::vector<int> t_cndls;     // 8
std::vector<int> t_cndl;      // 9

auto start = std::chrono::steady_clock::now();
auto end = std::chrono::steady_clock::now();
auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

void saveTestResult(const std::vector<int> &df, const std::string &name) {
    double sum_df = 0;
    for (auto it : df)
        sum_df += it;
    sum_df /= (df.size() - 1);
    std::cout << ("Среднее время просчитывания " + name + ": ") << sum_df << " ms" << std::endl;
    std::fstream fout(LOG, std::ios::out | std::ios::app);
    if (fout.is_open()) {
        fout << (">" + name) << std::endl;
        fout << "-->" << sum_df << " ms" << std::endl;
        for(size_t i = 0; i < df.size(); ++i)
            fout << "--->"<< i << " - " << df[i] << " ms" << std::endl;
    }
    fout.close();
}
#endif

class RtMOEX {
protected:
    const std::string HOST = "iss.moex.com";  // ссылка на MOEX

    std::string history;  // История/настоящие время
    std::string engine;  // Доступные торговые системы
    std::string market;  // Рынки торговой системы
    std::string session;  // Список сессий доступных в итогах торгов. Только для фондового рынка!
    std::string board;  // Режимы торгов рынка

    [[nodiscard]] virtual inline const std::string histoty_status() const {  return this->history; }
    [[nodiscard]] virtual inline const std::string egnines_status() const {  return this->engine; }
    [[nodiscard]] virtual inline const std::string markets_status() const {  return this->market; }
    [[nodiscard]] virtual inline const std::string sessions_status() const {  return this->session; }
    [[nodiscard]] virtual inline const std::string boards_status() const {  return this->board; }

    /** \brief - метод формирует запрос к серверу, подставляет нужные параметры.
     * \param SECID - акция
     * \param first - первая дата
     * \param last - вторая дата
     * */
    inline const std::string get_target_form(const std::string &SECID, Date first, Date last) {
        std::string target;
        target += "/iss" + histoty_status();
        target += "/engines/" + egnines_status();
        target += "/markets/" + markets_status();
        if (!histoty_status().empty())
            target += "/sessions/" + sessions_status();
        target += "/boards/" + boards_status();
        target += "/securities/" + SECID + ".xml?iss.meta=off&";
        target += (histoty_status().empty()) ? "iss.only=marketdata" : "iss.only=history";
        if (!histoty_status().empty()) {
            target += "&from=" + first.date();
            target += "&till=" + last.date();
        }
        // saveas(target, "target.txt");
        return target;
    }

    std::string get_request_to_MOEX_in_the_format_xml(const std::string &SECID, Date first = Date(), Date last = Date()) {
#if TEST_METHODS
        start = std::chrono::steady_clock::now();
#endif
        if (first == Date())
            --first;
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_day.push_back(t);
#endif
        namespace http = boost::beast::http;

        const std::string host = HOST;
#if TEST_METHODS
        start = std::chrono::steady_clock::now();
#endif
        const std::string target = get_target_form(SECID, first, last);
        saveas(target, "target.txt");
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_target.push_back(t);
#endif
        // I/O контекст, необходимый для всех I/O операций
        boost::asio::io_context ioc;

        // Resolver для определения endpoint'ов
        boost::asio::ip::tcp::resolver resolver(ioc);
        // Tcp сокет, использующейся для соединения
        boost::asio::ip::tcp::socket socket(ioc);

        // Резолвим адрес и устанавливаем соединение
        boost::asio::connect(socket, resolver.resolve(host, "80"));

        // Дальше необходимо создать HTTP GET реквест с указанием таргета
        http::request <http::string_body> req(http::verb::get, target, 11);
        // Задаём поля HTTP заголовка
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Отправляем реквест через приконекченный сокет
        http::write(socket, req);

        // Часть, отвечающая за чтение респонса
        std::string res_str;
        {
            boost::beast::flat_buffer buffer;
            http::response <http::dynamic_body> res;
            http::read(socket, buffer, res);
            res_str = boost::beast::buffers_to_string(res.body().data());
        }
        // Закрываем соединение
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        return res_str;
    }

    std::vector<std::string> split_the_request_into_rows(const std::string &df) {
#if TEST_METHODS
        start = std::chrono::steady_clock::now();
#endif
        std::string s = cut(df, "<row ");
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_cut.push_back(t);
        start = std::chrono::steady_clock::now();
#endif
        s = s.substr(0, s.find("</rows>"));
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_substr.push_back(t);

#endif
        return position::get(s);
    }


#include "Candle.h"

public:
    RtMOEX() = default;
    ~RtMOEX() {
#if TEST_METHODS
        saveTestResult(t_day, "просчет дня");
        saveTestResult(t_target,"просчет сборки ссылки");
        saveTestResult(t_cut,"вырезание начала строки");
        saveTestResult(t_substr,"вырезание конца строки");
        //saveTestResult(t_split,"время разбиения строки");
        saveTestResult(t_get,"время запроса");
        saveTestResult(t_pars,"время парсинга");
        saveTestResult(t_cndls,"время получения массива свеч");
        saveTestResult(t_cndl,"время получения одной свечи");
#endif
    }

    boost::variant<std::vector<Candle>, Candle> parser(const std::string &secid, Date first = Date(), Date last = Date()) {
        /*
         * Делаем get-запрос к серверу, чтобы получить информаци об акции. Информаци получаем ввиде единой строки,
         * в ней будет как лишняя, так и нужная информация.
         * */
#if TEST_METHODS
        start = std::chrono::steady_clock::now();
#endif
        std::string df = get_request_to_MOEX_in_the_format_xml(secid, first, last);
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_get.push_back(t); //
#endif
        /*
         * Очищаем полученный запрос от лишней информации и разбиваем его на строки
         * */
#if TEST_METHODS
        start = std::chrono::steady_clock::now();
#endif
        std::vector<std::string> parsed_df = split_the_request_into_rows(df);
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_pars.push_back(t); 
#endif
        /*
         * Получаем массив свечей, распарсив строку
         * */
#if TEST_METHODS
        start = std::chrono::steady_clock::now();
#endif
        if (!histoty_status().empty()) {
            std::vector<Candle> cndls;
            for (auto it : parsed_df) {
                cndls.push_back(Candle(Candle(float(std::atof(parser_in_data(it, "OPEN").c_str())),
                                              float(std::atof(parser_in_data(it, "CLOSE").c_str())),
                                              float(std::atof(parser_in_data(it, "LOW").c_str())),
                                              float(std::atof(parser_in_data(it, "HIGH").c_str()))), 
                                       Date(parser_in_data(it, "TRADEDATE")))); // this
            }
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_cndls.push_back(t); //       start = std::chrono::steady_cloc
#endif
            return cndls;
        }
#if TEST_METHODS
        start = std::chrono::steady_clock::now();

#endif  
        Candle cndl(Candle(float(std::atof(parser_in_data(parsed_df[0], "OPEN").c_str())),
                           float(std::atof(parser_in_data(parsed_df[0], "CLOSE").c_str())),
                           float(std::atof(parser_in_data(parsed_df[0], "LOW").c_str())),
                           float(std::atof(parser_in_data(parsed_df[0], "HIGH").c_str()))),
                    Time(parser_in_data(parsed_df[0], "TIME"))); // this
#if TEST_METHODS
        end = std::chrono::steady_clock::now();
        t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        t_cndl.push_back(t); // start = std::chrono::steady_clock::now();

#endif
        return cndl;
    }
};


#endif //MOEXPARSER_REQUEST_TO_MOEX_H
