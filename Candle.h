#ifndef MOEXPARSER_CANDLE_H
#define MOEXPARSER_CANDLE_H

#include <boost/variant.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "Date.h"
#include "Time.h"


class _no_element : public std::exception {
    std::string error;
public:
    _no_element(const std::string& msg) : error(msg) {}

    virtual const char* what() const noexcept {
        return error.c_str();
    }

};

/** \class Candle
 * \param date - дата свечи
 *     DATA  - Дата свечи
 * \param df_candle - тело свечи
 *     OPEN  - Цена открытия свечи
 *     CLOSE - Цена закрытия свечи
 *     LOW   - Минимальная цена в свече
 *     HIGH  - Маскимальная цена в свече
 * */
class Candle {
    Date date;
    Time time;
    std::map<const char*, float> df_candle;

public:
    Candle( const float &OPEN, const float &CLOSE, const float &LOW, const float &HIGH) {
        df_candle["OPEN"] = OPEN;
        df_candle["CLOSE"] = CLOSE;
        df_candle["LOW"] = LOW;
        df_candle["HIGH"] = HIGH;
    }

    Candle(const Candle &cl) {
        date = cl.date;
        time = cl.time;
        df_candle = cl.df_candle;
    }

    Candle(const Candle &cl, const Time &t) : df_candle(cl.df_candle), time(t) {}
    Candle(const Candle &cl, const Date &d) : df_candle(cl.df_candle), date(d) {}
    Candle(const Candle &cl, const Time &t, const Date &d) : df_candle(cl.df_candle), time(t), date(d) {}

    Candle() = default;

    Candle& operator=(const Candle &cl) {
        if (*this == cl)
            return *this;
        date = cl.date;
        time = cl.time;
        df_candle = cl.df_candle;
        return *this;
    }

    ~Candle() {
        df_candle.clear();
    }

    bool operator==(const Candle &cl) {
        if (time != cl.time)
            return false;
        if (date != cl.date)
            return false;
        return (this->df_candle == cl.df_candle) ? true : false;
    }

    boost::variant<float, Date, Time>  operator[] (const std::string parameter) {
        if (parameter == "DATE")
            return this->date;
        else if (parameter == "TIME")
            return this->time;
        else if (parameter == "OPEN" || parameter == "CLOSE" || parameter == "LOW" || parameter == "HIGH")
            return this->df_candle[parameter.c_str()];
        else
            throw _no_element(parameter);
    }
};


#endif //MOEXPARSER_CANDLE_H
