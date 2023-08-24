#ifndef FINALPROJECT_CONVERTERJSON_H
#define FINALPROJECT_CONVERTERJSON_H

#pragma once
#include "nlohmann/json.hpp"
#include "vector"
#include "fstream"
#include "iostream"
#include "string"
#include "sstream"
#include "map"
#include "mutex"

struct Answer
{
    std::string requestNum;
    std::map<int, double> result;
    bool flag{};
};

class failFileOpenException : public std::exception
{
    [[nodiscard]] const char* what() const noexcept override
    {
        return "No file 'config.json'!";
    }
};

class failConfigException : public std::exception
{
    [[nodiscard]] const char* what() const noexcept override
    {
        return "No field 'config' in 'config.json'!";
    }
};

class failEmptyDataBase : public std::exception
{
    [[nodiscard]] const char* what() const noexcept override
    {
        return "All documents in search base are empty!";
    }
};

class ConverterJson
{
public:
    ConverterJson();
    /*
     * конструктор по умолчанию при вызове считывает информацию из файлов 'config.json' и 'request.json' и
     * заполняет соответствующие переменные configFile и requestFile
    */

    ConverterJson (std::vector<std::string> &inDocs, std::vector<std::string> &inReqs)
    {
           textFromDocs = inDocs;
           requests = inReqs;
    };
    /*
     * конструктор для создания экземпляров класса ConverterJson при проведении тестирования элементов программы,
     * на вход принимает два вектора строк, используемых для инициализации переменных  textFromDocs и requests
    */

    std::vector<std::string> GetTextDocuments();
    /*
     * метод получает данные из базы данных документов, возвращает вектор строк,
     * каждая строка (элемент вектора) - весь текст из отдельного документа из базы.
     * возвращает vector<string> textFromDocs
    */

    int GetFilesNum();
    /*
     * метод вычисляет количество документов в базе, вычисляя размер вектора textFromDocs,
     * данная информация используется далее в методе preRelevanceFill() класса SearchServer
    */

    int GetResponseLimit() const;
    /*
     * метод возвращает значение переменной respLimit
    */

    std::vector<std::string> GetRequestsData();
    /*
     * метод возвращает вектор строк, содержащих данные о запросах
     * return const std::vector<std::string> res = requests
    */

    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
    /*
     * метод осуществляет запись результата поиска в файл 'answers.json', принимает на
     * вход вектор с результатами обработки запросов, поэтому размер входящего вектора равен числу запросов.
     * Каждый элемент входящего вектора - также вектор, содержащий информацию о резульататах запроса для
     * всех найденных документов с ненулевой релевантностью по данному запросу
    */

private:
    void readConfig();
    /*
     * метод осуществляет чтение данных из переменной configFile, проверяет наличие в ней поля config и при его
     * отсутствии выбрасывает исключение, а также заполняет вектор filePaths, содержащий адреса до файлов с текстами,
     * в которых будет осуществляться поиск по запросам
    */

    void ResponseLimit();
    /*
     * метод устанавливает значение переменной int respLimit. Значение переменной определяется при считывании числа из
     * поля 'max_responses' файла 'config.json'. Если такого поля в файле нет, то устанавливается значение по умолчанию,
     * равное 5
    */

    std::vector<std::string> GetRequests();
    /*
     * метод возвращает значения вектора requests, содержащего данные о запросах из файла 'request.json'
    */

    int numOfFiles=0;
    int respLimit=5;
    std::vector<std::string> textFromDocs{};
    std::vector<std::string> requests{};
    std::vector<std::string> filePaths;

    nlohmann::json configFile;
    nlohmann::json requestFile;
    nlohmann::json answerFile;
    Answer ans;
};

#endif //FINALPROJECT_CONVERTERJSON_H
