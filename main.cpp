#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

namespace http = boost::beast::http;

// http://iss.moex.com/iss/history/engines/stock/markets/shares/boa
//rds/tqbr/securities.xml?date=2013-12-20

int main() {
    const std::string host = "iss.moex.com";
    const std::string target = "/iss/history/engines/stock/markets/shares/boards/tqbr/securities.xml?date=2013-12-20";

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

        std::cout << res << std::endl;
    }
    // Закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);

    return 0;
}