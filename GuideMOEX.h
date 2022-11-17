#ifndef MOEXPARSER_GUIDE_MOEX_H
#define MOEXPARSER_GUIDE_MOEX_H


#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/property_tree/detail/rapidxml.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <vector>
#include <map>
#include "Date.h"
#include "Time.h"
#include "utilities.h"

/** \bref guideMOEX - справочник по MOEX. \
 * Про то какие торговые системы есть, торговые рынки/сессии/режими
 * */
class guideMOEX {
protected:
    const std::string HOST = "iss.moex.com";  // ссылка на MOEX

    std::string get_request_to_MOEX_in_the_format_xml(const std::string &quest) {
        namespace http = boost::beast::http;

        const std::string host = HOST;
        const std::string target = quest;

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
        return position::get(s, false);
    }

    std::vector<std::pair<std::string, std::string>> request(const std::string &req, const std::string &n, const std::string &t) {
        std::string df = get_request_to_MOEX_in_the_format_xml(req);
        std::vector<std::string> cdf = split_the_request_into_rows(df);
        std::vector<std::pair<std::string, std::string>> obj;
        for (auto it : cdf)
            obj.push_back(std::pair(parser_in_data(it, n).c_str(), parser_in_data(it, t).c_str()));
        return obj;
    }


public:
    guideMOEX() = default;
    ~guideMOEX() = default;

    /** \bref -  доступные торговые системы
     * */
    std::vector<std::pair<std::string, std::string>> engines() {
        std::string req = "/iss/engines.xml?iss.meta=off";
        return request(req, "name", "title");
    }

    /** \bref -  рынки торговой системы
     *  \param engine - доступные торговые системы
     * */
    std::vector<std::pair<std::string, std::string>> markets(const std::string &engine) {
        std::string req = ("/iss/engines/" + engine + "/markets.xml?iss.meta=off");
        return request(req, "NAME", "title");
    }

    /** \bref - Режимы торгов рынка
     *  \param engine - доступные торговые системы
     *  \param market - рынки торговой системы 
     * */
    std::vector<std::pair<std::string, std::string>> boards(const std::string &engine, const std::string &market) {
        std::string req = ("/iss/engines/" + engine + "/markets/" + market + "/boards.xml?iss.meta=off"); 
        return request(req, "boardid", "title");
    }

    /** \bref - Список сессий доступных в итогах торгов; Только для фондового рынка!
     *  \param engine - доступные торговые системы
     *  \param market - рынки торговой системы 
     * */ 
    std::vector<std::pair<std::string, std::string>> sessions(const std::string &engine, const std::string &market) {
        std::string req = ("/iss/engines/" + engine + "/markets/" + market + "/sessions.xml?iss.meta=off");
        return request(req, "id", "title");
    }

    /* Дополнительно можно еще получать типы и группы ценных бумаг, но как я пришел к выводу - они нигде не используются.
     * При запросе к историческим или дневным данным вот это не используют. В диведентах это тоже не участвует,
     * смысла особо нет запрашивать/реализовывать, оставлю реализацию на усмотрение комьюнити...
     *
     * // Группы ценных бумаг
     * https://iss.moex.com/iss/securitygroups.xml
     *
     * // Типы ценных бумаг
     * https://iss.moex.com/iss/securitytypes.xml
     * */
};


#endif //MOEXPARSER_GUIDE_MOEX_H
