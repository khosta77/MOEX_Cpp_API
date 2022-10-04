#ifndef MOEXPARSER_SL_MOEX_H
#define MOEXPARSER_SL_MOEX_H

#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <fstream>
#include "Date.h"
#include "utilities.h"

const std::vector<std::string> IMOEX = {
        "AFKS", "AFLT", "ALRS", "CBOM", "CHMF", "DSKY"
};  // https://ru.tradingview.com/symbols/MOEX-IMOEX/components/

namespace http = boost::beast::http;
class MOEX_parser {
    const std::string HOST = "iss.moex.com";

     void getMoexXml(const std::string &SECID, Day first = Day(), Day last = Day()) {
        if (first == Day()) {
            first.prev();
        }
        const std::string host = HOST;
        const std::string target = "/iss/history/engines/stock/markets/shares/boards/TQBR/securities/" + \
                                   SECID + ".xml?from=" + first.date() + \
                                   "&till=" + last.date() + "&iss.meta=off";
                // "/iss/history/engines/stock/markets/shares/boards/tqbr/securities.xml?date=" + date;

        // I/O контекст, необходимый для всех I/O операций
        boost::asio::io_context ioc;

        // Resolver для определения endpoint'ов
        boost::asio::ip::tcp::resolver resolver(ioc);
        // Tcp сокет, использующейся для соединения
        boost::asio::ip::tcp::socket socket(ioc);

        // Резолвим адрес и устанавливаем соединение
        boost::asio::connect(socket, resolver.resolve(host, "80"));

        // Дальше необходимо создать HTTP GET реквест с указанием таргета
        http::request<http::string_body> req(http::verb::get, target, 11);
        // Задаём поля HTTP заголовка
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Отправляем реквест через приконекченный сокет
        http::write(socket, req);

        // Часть, отвечающая за чтение респонса
        {
            boost::beast::flat_buffer buffer;
            http::response<http::dynamic_body> res;
            http::read(socket, buffer, res);
            std::fstream fout("status.xml", std::ios::out);
            if (fout.is_open()) {
                fout << res << std::endl;
                fout.close();
            }
        }
        // Закрываем соединение
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    }


#include "candle.h"

public:
    MOEX_parser() = default;
    ~MOEX_parser() = default;

    Candle parser(const std::string &secid) {
        // Вот тут проверка на акцию

        getMoexXml(secid);
        std::fstream fin("status.xml", std::ios::in);
        std::string s{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
//        system("rm -rf ./status.xml");  // Команда нужна для очистки системы от ненужного файла
        using namespace std;

        auto s2 = cut(s, "<row ");
        s2 = s2.substr(0, s2.find("</rows>"));

        auto pos = position::get(s2);
        Candle candle(parser_in_data(pos[0], "TRADEDATE"),
                  float(atof(parser_in_data(pos[0], "OPEN").c_str())),
                  float(atof(parser_in_data(pos[0], "CLOSE").c_str())),
                  float(atof(parser_in_data(pos[0], "LOW").c_str())),
                  float(atof(parser_in_data(pos[0], "HIGH").c_str()))
                  );
        return candle;
    }
};

#endif //MOEXPARSER_SL_MOEX_H
