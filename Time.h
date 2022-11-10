#ifndef MOEXPARSER_TIME_H
#define MOEXPARSER_TIME_H

#include <boost/tokenizer.hpp>
#include <string>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>


struct Time {
    unsigned char hours;
    unsigned char minutes;
    unsigned char seconds;

    // constructor & destructor
    Time() {  // Получаем время в данный момент
        std::time_t now = std::time(0);
        tm *hms = localtime(&now);
        this->hours = hms->tm_hour;
        this->minutes = hms->tm_min;
        this->seconds = hms->tm_sec;
    }

    Time(const unsigned char &h, const unsigned char &m, const unsigned char &s) : hours(h), minutes(m), seconds(s) {}

    Time(const std::string &dt) {
        this->hours = std::atoi(dt.substr(0, 2).c_str());
        this->minutes = std::atoi(dt.substr(3, 2).c_str());
        this->seconds = std::atoi(dt.substr(6, 2).c_str());
    }

    Time(const Time& dt) : hours(dt.hours), minutes(dt.minutes), seconds(dt.seconds) {}

    Time& operator= (const Time& dt) {
        if (*this == dt)
            return *this;
        this->hours = dt.hours;
        this->minutes = dt.minutes;
        this->seconds = dt.seconds;
        return *this;
    }

    ~Time() = default;

    // methods
    inline const std::string time() {
        return (std::to_string(this->hours) + ":" + std::to_string(this->minutes) + ":" + std::to_string(this->seconds));
    }

    inline const bool operator==(const Time &rh) {
        return (this->hours == rh.hours && this->minutes == rh.minutes && this->seconds == rh.seconds) ? true : false;
    }

    inline const bool operator!=(const Time &rh) {  return !(*this == rh); }

};


#endif //MOEXPARSER_TIME_H
