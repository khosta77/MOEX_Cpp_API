#include "utilities.h"

std::string cut(const std::string &str, const char *find) {
    std::string::size_type pos = str.find(find);
    if (pos != std::string::npos)
        return str.substr(pos);
    return std::string();
}

std::string parser_in_data(const std::string &str, const std::string &find) {
    std::string pr;
    for (size_t i = (str.find(" " + find) + find.size() + 3); str[i] != '\"'; ++i) {
        pr += str[i];
    }
    return pr;
}

std::vector<std::string> position::get(std::string &str, const bool &fwsic) {
    std::vector<std::string> pos;
    for (size_t i = 0; i < std::count(str.begin(), str.end(), '>'); ++i) {
        pos.push_back(str.substr(0, (str.find('>') + 1)));
        str = cut(cut(str, "\n"), "<");
    }
    filter_whitespace(pos);
    if(fwsic)
        filter_whitespace_in_cell(pos);
    return pos;
}

void position::filter_whitespace(std::vector<std::string> &pos) {
    int k = 0;
    for (auto &it: pos) { // это можно красиво сделать
        if (it.find("<") == 0)
            break;
        ++k;
    }
    pos.erase(pos.begin(), (pos.begin() + k));
}

void position::filter_whitespace_in_cell(std::vector<std::string> &pos) {
    std::vector<std::string> pos_filt;
    for (auto &it : pos)
        if (atof(parser_in_data(it, "OPEN").c_str()) != 0)
            pos_filt.push_back(it);
    std::swap(pos, pos_filt);
    pos_filt.clear();
}

void saveas(const std::string &date, const std::string &fn) {
    std::fstream fout(fn.c_str(), std::ios::out);
    if (fout.is_open()) {
        fout << date << std::endl;
    }
    fout.close();
}