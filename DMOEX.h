#ifndef MOEXPARSER_DMOEX_H
#define MOEXPARSER_DMOEX_H

#include "RtMOEX.h"

class moex_day : public RtMOEX {
    std::string history;  // История/настоящие время
    std::string engines;  // Доступные торговые системы
    std::string markets;  // Рынки торговой системы
    std::string sessions;  // Список сессий доступных в итогах торгов. Только для фондового рынка!
    std::string boards;  // Режимы торгов рынка

    [[nodiscard]] inline const std::string histoty_status() const override {  return this->history; }
    [[nodiscard]] inline const std::string egnines_status() const override {  return this->engines; }
    [[nodiscard]] inline const std::string markets_status() const override {  return this->markets; }
    [[nodiscard]] inline const std::string sessions_status() const override {  return this->sessions; }
    [[nodiscard]] inline const std::string boards_status() const override {  return this->boards; }
public:
    moex_day(const std::string &engine = "stock", const std::string &market = "shares",
                 const std::string &session = "3", const std::string &board = "TQBR") {
        this->history = "";
        this->engines = engine;
        this->markets = market;
        this->sessions = session;
        this->boards = board;
    }
};

#endif //MOEXPARSER_DMOEX_H
