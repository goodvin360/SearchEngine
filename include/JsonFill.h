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

struct AnswerFile
{

};

class JsonFill
{
private:
    ConfigFile configExample;
    RequestFile requestExample;
    AnswerFile answerExample;

public:
    void fillFile(std::string type);
    void printFile(std::string type, std::string path);
};

#endif //FINALPROJECT_JSONFILL_H
