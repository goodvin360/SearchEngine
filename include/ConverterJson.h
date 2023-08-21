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
    bool flag;
};

class failFileOpenException : public std::exception
{
    const char* what() const noexcept override
    {
        return "No file 'config.json'!";
    }
};

class failConfigException : public std::exception
{
    const char* what() const noexcept override
    {
        return "No field 'config' in 'config.json'!";
    }
};

class failEmptyDataBase : public std::exception
{
    const char* what() const noexcept override
    {
        return "All documents in search base are empty!";
    }
};

class ConverterJson
{
public:
    ConverterJson();
    ConverterJson (std::vector<std::string> &inDocs, std::vector<std::string> &inReqs)
    {
           textFromDocs = inDocs;
           requests = inReqs;
    };

    std::vector<std::string> GetTextDocuments();
    int GetFilesNum();
    int GetResponseLimit();
    const std::vector<std::string> GetRequestsData();
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);


private:
    void readConfig();
    void ResponseLimit();
    std::vector<std::string> GetRequests();
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
