#ifndef MOEXPARSER_CANDLE_H
#define MOEXPARSER_CANDLE_H

#include <boost/variant.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "Date.h"

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
    std::map<const char*, float> df_candle;

public:
    Candle(const std::string &DATE, const float &OPEN, const float &CLOSE, const float &LOW, const float &HIGH) :
            date(DATE) {
        df_candle["OPEN"] = OPEN;
        df_candle["CLOSE"] = CLOSE;
        df_candle["LOW"] = LOW;
        df_candle["HIGH"] = HIGH;
    }

    Candle(const Candle &cl) {
        date = cl.date;
        df_candle = cl.df_candle;
    }

    Candle& operator=(const Candle &cl) {
        if (*this == cl)
            return *this;
        date = cl.date;
        df_candle = cl.df_candle;
        return *this;
    }

    ~Candle() {
        df_candle.clear();
    }

    bool operator==(const Candle &cl) {
        if (date != cl.date)
            return false;
        return (this->df_candle == cl.df_candle) ? true : false;
    }

    boost::variant<float, Date>  operator[] (const std::string parameter) {
        if (parameter == "DATE")
            return this->date;
        else if (parameter == "OPEN" || parameter == "CLOSE" || parameter == "LOW" || parameter == "HIGH")
            return this->df_candle[parameter.c_str()];
        else
            throw _no_element(parameter);
    }
};

typedef std::vector<Candle> Candles;

#endif //MOEXPARSER_CANDLE_H
