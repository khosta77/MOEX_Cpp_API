#ifndef MOEXPARSER_REQUEST_TO_MOEX_H
#define MOEXPARSER_REQUEST_TO_MOEX_H

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/variant.hpp>
#include <boost/property_tree/detail/rapidxml.hpp>
#include <iostream>
#include <fstream>
#include "Date.h"
#include "utilities.h"

#define MRK "\n=========================================================================================================="
//const std::vector<std::string> IMOEX = {
//        "AFKS", "AFLT", "ALRS", "CBOM", "CHMF", "DSKY"
//};  // https://ru.tradingview.com/symbols/MOEX-IMOEX/components/

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
        target += (histoty_status().empty()) ? "iss.only=marketdata&" : "iss.only=history&";
        if (!histoty_status().empty()) {
            target += "from=" + first.date();
            target += "&till=" + last.date();
        }
        return target;
    }

    std::string get_request_to_MOEX_in_the_format_xml(const std::string &SECID, Date first = Date(), Date last = Date()) {
        if (first == Date())
            --first;

        namespace http = boost::beast::http;

        const std::string host = HOST;
        const std::string target = get_target_form(SECID, first, last);

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

    std::vector<std::string> dividerRows(const std::string &df) {
        std::string s = cut(df, "<row ");
        s = s.substr(0, s.find("</rows>"));
        return position::get(s);
    }


#include "Candle.h"

public:
    RtMOEX() = default;
    ~RtMOEX() = default;

    boost::variant<Candle, Candles> parser(const std::string &secid, Date first = Date(), Date last = Date()) {
        /*
         * Делаем get-запрос к серверу, чтобы получить информаци об акции. Информаци получаем ввиде единой строки,
         * в ней будет как лишняя, так и нужная информация.
         * */
        std::string df = get_request_to_MOEX_in_the_format_xml(secid, first, last);
//        saveas(df + MRK, "moexXML.txt"); // ТЕСТ ОТВЕТА
        /*
         *
         * */
        auto parsed_df = dividerRows(df);
        for (auto it : parsed_df)
            saveas(it, "parsed_df_xml.txt");
//        std::cout << parsed_df.size() << std::endl;


        Candle candle;
//        saveas(df, "status.xml");
//        using namespace std;
//        Candle candle;
//        system("rm -rf ./status.xml");  // Команда нужна для очистки системы от ненужного файла
//        Candle candle(parser_in_data(parsed_df[0], "TRADEDATE"),
//                      float(atof(parser_in_data(parsed_df[0], "OPEN").c_str())),
//                      float(atof(parser_in_data(parsed_df[0], "CLOSE").c_str())),
//                      float(atof(parser_in_data(parsed_df[0], "LOW").c_str())),
//                      float(atof(parser_in_data(parsed_df[0], "HIGH").c_str()))
//        );
        return candle;
    }
};


#endif //MOEXPARSER_REQUEST_TO_MOEX_H
