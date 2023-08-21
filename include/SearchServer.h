#ifndef FINALPROJECT_SEARCHSERVER_H
#define FINALPROJECT_SEARCHSERVER_H

#pragma once
#include "vector"
#include "iostream"
#include "map"
#include "InvertedIndex.h"
#include "ConverterJson.h"

class SearchServer
{
public:
    SearchServer(InvertedIndex& inIdx, ConverterJson &inJson): index(inIdx), dataJson(inJson) {};

    std::vector<std::vector<std::pair<int, float>>> searchFoo (const std::vector<std::string> &queries_input);

private:
    InvertedIndex index;
    ConverterJson dataJson;
    std::map<std::string,int> uniqRequests;
    std::multimap<int,int> preRelevance;
    std::vector<std::vector<std::pair<int, float>>> result;
    std::vector<std::pair<int, float>> relativeIndex;
    void uniqRequestsFill(const std::string& request, int reqNum);
    void preRelevanceFill();
    int findMaxAbsRel();
    int maxAbsRelevance = 0;
    std::vector<std::pair<int, float>> sortRelativeIndex();
};

#endif //FINALPROJECT_SEARCHSERVER_H
