#ifndef MOEXPARSER_UTILITIES_H
#define MOEXPARSER_UTILITIES_H

#include <string>
#include <vector>
#include <algorithm>

/** \brief Обрезает \param str до определенного \param find символа/объекта
 * \return возращается строка после параметра find
 * */
std::string cut(const std::string &str, const char *find);

/** \brief Находит в строке \param str объект \param find и возращает значение после него в ""
 * */
std::string parser_in_data(const std::string &str, const std::string &find);

struct position {
    static std::vector<std::string> get(std::string &str, const bool &fwsic = true);
private:
    static void filter_whitespace(std::vector<std::string> &pos);
    static void filter_whitespace_in_cell(std::vector<std::string> &pos);
};

#endif //MOEXPARSER_UTILITIES_H
