#include <boost/beast.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>
#include <fstream>
#include <regex>
#include <algorithm>

namespace http = boost::beast::http;

// http://iss.moex.com/iss/history/engines/stock/markets/shares/boa
//rds/tqbr/securities.xml?date=2013-12-20

std::string cut(const std::string &str, const char* find) {
    std::string::size_type pos = str.find(find);
    if (pos != std::string::npos)
        return str.substr(pos);
    return std::string();
}

std::vector<std::string> getPosition(std::string &str) {
    const auto SIZE = std::count(str.begin(), str.end(), '>');
    std::vector<std::string> pos(SIZE);
    for (size_t i = 0; i < SIZE; ++i) {
//        n = str.find('>') + 1;
        pos.push_back(str.substr(0, (str.find('>') + 1)));
//        std::cout << str.substr(0,  n);
        str = cut(str, "\n");
        str = cut(str, "<");
    }
    return pos;
}

int main() {
    /*
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
        std::fstream fout("status.xml", std::ios::out);
        if (fout.is_open()) {
            fout << res << std::endl;
            fout.close();
        }
//        std::string s ;
//        std::cout << s << std::endl;
    }
    // Закрываем соединение
    socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
*/
    std::fstream fin("status.xml", std::ios::in);
    std::string s{std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>()};
    using namespace std;

//    s.substr(s.find("<"));
    auto s2 = cut(s, "<row ");
    s2 = s2.substr(0, s2.find("</rows>"));
    cout << s2 << endl;

    auto pos = getPosition(s2);
    for (auto i : pos) {
        cout << i << endl;
    }
//    std::regex r("<rows>(\\d+)</rows>");
//    std::smatch m;
//
//    while (std::regex_search (s2, m, r))
//    {
//        std::cout << m[1] << std::endl;
////        s = m.suffix().str();
//    }

    return 0;
}