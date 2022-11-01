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

//const std::vector<std::string> IMOEX = {
//        "AFKS", "AFLT", "ALRS", "CBOM", "CHMF", "DSKY"
//};  // https://ru.tradingview.com/symbols/MOEX-IMOEX/components/

class RtMOEX {
    const std::string HOST = "iss.moex.com";  // ссылка на MOEX

    [[maybe_unused]] std::string history;  // История/настоящие время
    [[maybe_unused]] std::string engines;  // Доступные торговые системы
    [[maybe_unused]] std::string markets;  // Рынки торговой системы
    [[maybe_unused]] std::string sessions;  // Список сессий доступных в итогах торгов. Только для фондового рынка!
    [[maybe_unused]] std::string boards;  // Режимы торгов рынка

    virtual inline const std::string histoty_status() const {  return this->history; }
    virtual inline const std::string egnines_status() const {  return this->engines; }
    virtual inline const std::string markets_status() const {  return this->markets; }
    virtual inline const std::string sessions_status() const {  return this->sessions; }
    virtual inline const std::string boards_status() const {  return this->boards; }

    /** \brief - метод формирует запрос к серверу, подставляет нужные параметры.
     * \param SECID - акция
     * \param first - первая дата
     * \param last - вторая дата
     * */
    inline const std::string get_target_form(const std::string &SECID, Date first, Date last) {
        std::string target;
        target += "/iss" + histoty_status();
        target += "/engines" + egnines_status();
        target += "/markets" + markets_status();
        if (!histoty_status().empty())
            target += "/sessions" + sessions_status();
        target += "/boards" + boards_status();
        target += "/securities" + SECID + ".xml?iss.meta=off&";
        target += (histoty_status().empty()) ? "iss.only=marketdata&" : "iss.only=history&";
        if (!histoty_status().empty()) {
            target += "from=" + first.date();
            target += "&till=" + last.date();
        }
        return target;
    }

    std::string getMoexXml(const std::string &SECID, Date first = Date(), Date last = Date()) {
        if (first == Day())
            first.prev();

        namespace http = boost::beast::http;

        const std::string host = HOST;
        const std::string target = get_target_form(SECID, first, last)

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

    Candle parser(const std::string &secid) {
        // Вот тут проверка на акцию

        auto df = getMoexXml(secid);
        auto parsed_df = dividerRows(df);
//        saveas(df, "status.xml");
        using namespace std;
//        Candle candle;
//        system("rm -rf ./status.xml");  // Команда нужна для очистки системы от ненужного файла
        Candle candle(parser_in_data(parsed_df[0], "TRADEDATE"),
                      float(atof(parser_in_data(parsed_df[0], "OPEN").c_str())),
                      float(atof(parser_in_data(parsed_df[0], "CLOSE").c_str())),
                      float(atof(parser_in_data(parsed_df[0], "LOW").c_str())),
                      float(atof(parser_in_data(parsed_df[0], "HIGH").c_str()))
        );
        return candle;
    }
};


#endif //MOEXPARSER_REQUEST_TO_MOEX_H
