#ifndef MOEXPARSER_CANDLE_H
#define MOEXPARSER_CANDLE_H

#include <boost/variant.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class Candle {
    std::string date;
    std::map<std::string, float> df_candle;

    class _no_element : public std::exception {
        std::string error;
    public:
        _no_element(const std::string& msg) : error(msg) {}

        virtual const char* what() const noexcept {
            return error.c_str();
        }

    };

public:
    Candle() = default;
    Candle(const std::string &DATE, const float &OPEN, const float &CLOSE, const float &LOW, const float &HIGH) :
            date(DATE) {
        df_candle["OPEN"] = OPEN;
        df_candle["CLOSE"] = CLOSE;
        df_candle["LOW"] = LOW;
        df_candle["HIGH"] = HIGH;
    }
    ~Candle() {
        date.clear();
    }

    boost::variant<float, std::string>  operator[] (const std::string &parameter){
        if (parameter == "DATE") {
            return this->date;
        } else if (parameter == "OPEN" || parameter == "CLOSE" || parameter == "LOW" || parameter == "HIGH"){
            return this->df_candle[parameter];
        } else {
            throw _no_element(parameter);
        }
    }

};

#endif //MOEXPARSER_CANDLE_H
