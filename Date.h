#ifndef MOEXPARSER_DATE_H
#define MOEXPARSER_DATE_H

#include <boost/tokenizer.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

class Error : std::exception {
public:
    Error() = default;
};

struct Day {
    std::string year, month, day;
    int working_day;

    Day() {
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        year = std::to_string(now->tm_year + 1900);
        month = std::to_string(now->tm_mon + 1);
        day = std::to_string(now->tm_mday);
        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string s = std::ctime(&time);
        boost::tokenizer<> tok(s);
        auto d = *tok.begin();
        working_day = (d == "Mon" || d == "Tue" || d == "Wed" || d == "Thu" || "Fri") ? 1 : -1;
    }

    Day(const std::string &y, const std::string &m, const std::string &d) : year(y), month(m), day(d) {
        struct tm tm;
        if (!strptime(this->date().c_str(), "%Y-%m-%d", &tm))
            throw Error();
        this->working_day = 0;
    }

    ~Day() {
        year.clear();
        month.clear();
        day.clear();
    }

    inline const std::string date() {
        return (year + "-" + month + "-" + day);
    }

    inline const int status() {
        return this->working_day;
    }

    void next() {
        day = std::to_string((atoi(day.c_str()) + 1));
        try {
            Day test(year, month, day);
        } catch (Error &e) {
            day = "1";
            month = std::to_string((atoi(month.c_str()) + 1));
            try {
                Day test(year, month, day);
            } catch (Error &e) {
                year = std::to_string((atoi(year.c_str()) + 1));
                month = "1";
                day = "1";
            }
        }
    }

#if 0
/* 2022-10-04 18:46
 * // TODO: сделать так чтобы было можно идти и к обратному числу
 * */
    void prev() {
        day = std::to_string((atoi(day.c_str()) - 1));
        try {
            Day test(year, month, day);
        } catch (Error &e) {
            day = "1";
            month = std::to_string((atoi(month.c_str()) + 1));
            try {
                Day test(year, month, day);
            } catch (Error &e) {
                year = std::to_string((atoi(year.c_str()) + 1));
                month = "1";
                day = "1";
            }
        }
    }
#endif
};

struct Today : Day {
    std::string time;

    Today() = default;
    ~Today() = default;
    const std::string checkTime() {
        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string s = std::ctime(&t);
        boost::tokenizer<> tok(s);
        auto d = tok.begin();
        ++d; ++d; ++d;
        time += *d + ":"; ++d;
        time += *d + ":"; ++d;
        time += *d;
        return time;
    }

    inline const std::string getTime() noexcept { return time; }
};

#endif //MOEXPARSER_DATE_H
