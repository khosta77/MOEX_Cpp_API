#ifndef MOEXPARSER_DATE_H
#define MOEXPARSER_DATE_H

#include <boost/tokenizer.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

class The_error_of_the_existence_of_the_day : std::exception {
public:
    The_error_of_the_existence_of_the_day() = default;
};

struct Date {
    int year;
    unsigned char month;
    unsigned char day;
    int working_day;

    // constructor & destructor
    Date() {  // Получает сегодняшнею дату
        std::time_t t = std::time(0);
        std::tm* now = std::localtime(&t);
        this->year = (now->tm_year + 1900);
        this->month = (now->tm_mon + 1);
        this->day = (now->tm_mday);
//        std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string s = std::ctime(&t);
        boost::tokenizer<> tok(s);
        auto d = *tok.begin();
        this->working_day = (d == "Mon" || d == "Tue" || d == "Wed" || d == "Thu" || d == "Fri") ? 1 : -1;
    }

    Date(const int &y, const int &m, const int &d) : year(y), month(m), day(d) {
        struct tm tm;
        if (!strptime(this->date().c_str(), "%Y-%m-%d", &tm))  // Проверка на то существует ли день или нет
            throw The_error_of_the_existence_of_the_day();
        this->working_day = 0;
    }

    Date(const std::string &dt) {
        std::string buffer;
        buffer = (dt[0] + dt[1] + dt[2] + dt[3]);
        year = atoi(buffer.c_str());
        month = 0; day = 0;
        buffer = "";
        for(size_t i = 5; i < dt.size(); ++i) {
            if (dt[i] == '-') {
                month = (month != 0) ? month : atoi(buffer.c_str());
                day = atoi(buffer.c_str());
                buffer = "";
            } else
                buffer += dt[i];
        }
        working_day = 0;
        [[maybe_unused]] Date test(year, month, day);
    }

    Date(const Date& dt) : year(dt.year), month(dt.month), day(dt.day), working_day(dt.working_day) {}

    Date& operator= (const Date& dt) {
        if (*this == dt)
            return *this;
        year = dt.year;
        month = dt.month;
        day = dt.day;
        working_day = dt.working_day;
        return *this;
    }

    ~Date() = default;

    // methods
    inline std::string date() {
        return (std::to_string(this->year) + "-" + std::to_string(this->month) + "-" + std::to_string(this->day));
    }

    inline const int status() {  return this->working_day; }

    bool operator==(const Date &rh) {
        return (this->day == rh.day && this->month == rh.month && this->year == rh.year) ? true : false;
    }

    bool operator!=(const Data &rh) {  return !(*this == rh); }

    Data operator++() {
        next();
        return *this;
    }
    Data operator++(int) {
        Data b = this;
        next();
        return b;
    }
    Data operator--() {
        prev();
        return *this;
    }
    Data operator--(int) {
        Date b = this;
        prev();
        return b;
    }

private:
    void next() {
        ++day;
        try {
            [[maybe_unused]] Date test(year, month, day);
        } catch (The_error_of_the_existence_of_the_day &e) {
            day = 1;
            ++month;
            try {
                [[maybe_unused]] Date test(year, month, day);
            } catch (The_error_of_the_existence_of_the_day &e) {
                ++year;
                month = 1;
                day = 1;
            }
        }
    }

    void prev() {
        --day;
        if (day == 0)
            --month;
        else
            return;

        if (month == 0) {
            --year;
            month = 12; day = 31;
            return;
        }

        for (int i = 31; i > 27; --i) {
            try {
                [[maybe_unused]] Date test(year, month, i);
                day = i;
                return;
            } catch (The_error_of_the_existence_of_the_day &e) { }
        }
    }
};

//struct Today : Day {
//    std::string time;
//
//    Today() = default;
//    ~Today() = default;
//    const std::string checkTime() {
//        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//        std::string s = std::ctime(&t);
//        boost::tokenizer<> tok(s);
//        auto d = tok.begin();
//        ++d; ++d; ++d;
//        time += *d + ":"; ++d;
//        time += *d + ":"; ++d;
//        time += *d;
//        return time;
//    }
//
//    inline const std::string getTime() noexcept { return time; }
//};

#endif //MOEXPARSER_DATE_H
