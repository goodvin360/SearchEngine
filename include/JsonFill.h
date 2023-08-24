#ifndef FINALPROJECT_JSONFILL_H
#define FINALPROJECT_JSONFILL_H

#pragma once
#include "nlohmann/json.hpp"
#include "map"
#include "iostream"
#include "fstream"
#include "vector"
#include "sstream"

struct ConfigFile
{
    std::map<std::string, std::string> config;
    std::vector<std::string> files;
};

struct RequestFile
{
    std::vector<std::string> requests;
};

class JsonFill
{
private:
    ConfigFile configExample;
    RequestFile requestExample;

public:
    void fillFile(const std::string& type);
    /*
     * вспомогательный метод для заполнения файлов 'config.json' и 'request.json' программными средствами. Итогом работы методов является
     * формирование указанных файлов в корневом каталоге проекта
    */
    void printFile(const std::string& type, const std::string& path);
    /*
     * вспомогательный метод для вывода в консоль данных из документов рассматриваемой базы
    */
};

#endif //FINALPROJECT_JSONFILL_H
