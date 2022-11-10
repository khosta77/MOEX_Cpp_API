#ifndef MOEXPARSER_GUIDE_MOEX_H
#define MOEXPARSER_GUIDE_MOEX_H


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


class RtMOEX {
protected:
    const std::string HOST = "iss.moex.com";  // ссылка на MOEX
// TODO: переделать
    std::string history;  // История/настоящие время
    std::string engine;  // Доступные торговые системы
    std::string market;  // Рынки торговой системы
    std::string session;  // Список сессий доступных в итогах торгов. Только для фондового рынка!
    std::string board;  // Режимы торгов рынка
// TODO: вырезать
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

    std::vector<std::string> split_the_request_into_rows(const std::string &df) {
        std::string s = cut(df, "<row ");
        s = s.substr(0, s.find("</rows>"));
        return position::get(s);
    }


//#include "Candle.h"

public:
    RtMOEX() = default;
    ~RtMOEX() = default;

    std::vector<std::string> parser(const std::string &secid);
};


#endif //MOEXPARSER_GUIDE_MOEX_H
