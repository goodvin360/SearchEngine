#ifndef FINALPROJECT_INVERTEDINDEX_H
#define FINALPROJECT_INVERTEDINDEX_H

#define MultiThread
//#define SingleThread

#pragma once
#include "vector"
#include "iostream"
#include "map"
#include "thread"
#include "mutex"
#include "ConverterJson.h"

struct Entry
{
    size_t doc_id, count;
};

class InvertedIndex
{
public:
    InvertedIndex(ConverterJson &inJson): jsonData(inJson) {};

    void UpdateDocumentBase(std::vector<std::string> input_docs);
    std::vector<Entry> GetWordCount (const std::string &word);
    void freqDictInfillThread(std::string &textFromDoc);
    void dataMerge();
    void threadsDistribution();
    bool characterCondition(char &symbol);
    bool wordCondition(char &symbol, char& nextSymbol);
    std::map<std::string, std::vector<Entry>>* getFreqDictionary();

private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::vector<std::string> docs;
    ConverterJson jsonData;
    std::vector<std::string> threads;
    std::vector<InvertedIndex>classParts;
};


#endif //FINALPROJECT_INVERTEDINDEX_H
