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
    explicit InvertedIndex(ConverterJson &inJson): jsonData(inJson) {};
    /*
     * конструктор по умолчанию при вызове принимает на вход ссылку на экземпляр класса ConverterJson и инициализирует
     * переменную jsonData для последующего доступа через неё к объектам и методам класса ConverterJson
    */

    void UpdateDocumentBase(std::vector<std::string> input_docs);
    /*
     * метод для заполнения вектора строк docs данными о текстах, в которых будет осуществляться поиск
    */

    std::vector<Entry> GetWordCount (const std::string &word);
    /*
     * метод анализа частоты встречаемости слова в пределах всех имеющихся в базе документов. Принимает на вход ссылку на
     * анализируемое слово, возвращает вектор, каждый элемент которого является экземпляром структуры с заполненными
     * полями о номере документа и частоте встречи в нем исследуемого слова
    */

    void threadsDistribution();
    /*
     * метод по распределению имеющейся базы данных документов по отдельным вичислительным потокам для ускорения
     * процедуры формирования частотного словаря - объекта freq_dictionary.
     * В данном методе сначала создается вектор экземпляров класса InvertedIndex по размеру соответствующий
     * количеству документов в базе,
     * далее создается вектор вычислительных потоков по размеру ранее созданного вектора экземпляров класса,
     * и при создании каждого потока, из каждого экземпляра класса InvertedIndex  вызывается метод
     * freqDictInfillThread(std::string &textFromDoc), с отправкой в него данных по одному документу
     * из обрабатываемой базы данных.
     *
     * Таким образом отдельный вычислительный поток заполняет только свой объект freq_dictionary
     * в рамках некоторого отдельного экземпляра класса InvertedIndex, находящегося в ранее созданном векторе этих экземпляров.
     *
     * В завершении метода (после уничтожения всех вычислительных потоков) вызывается метод dataMerge()
    */

    void freqDictInfillThread(std::string &textFromDoc);
    /*
     * метод по созданию частотного словаря в рамках работы одного вычислительного потока.
     * Вызывается в рамках отдельно созданного экземпляра класса InvertedIndex и принимает на вход строку -
     * текст одного из документов базы данных, после чего для каждого слова из этой строки вызывает метод
     * GetWordCount (const std::string &word) и далее заполняет объект freq_dictionary также только для данного
     * отдельно созданного экземпляра класса InvertedIndex
    */

    void dataMerge();
    /*
     * метод по объединению данных из отдельных частотных словарей freq_dictionary из экземпляров класса  InvertedIndex,
     * существующих в ранее созданном векторе данных экземпляров. Данные объединяются в один общий freq_dictionary.
    */

    static bool characterCondition(char &symbol);
    /*
     * метод для парсинга символов из документа или запроса с учетом знаков препинаниня,
    */

    static bool wordCondition(char &symbol, char& nextSymbol);
    /*
    * метод для парсинга текста из документа или запроса и разбивки его на отдельные слова с учетом знаков препинаниня
    * при возврате true отправляет слово для дальнейшей обработки.
    */


    std::map<std::string, std::vector<Entry>>* getFreqDictionary();
    /*
    * метод для получения данных о сформированном частотном словаре, используется для обработки данных из запросов в
     * методах класса SearchServer
     * возвращает ссылку на объект std::map<std::string, std::vector<Entry>> freq_dictionary;
    */

private:
    std::map<std::string, std::vector<Entry>> freq_dictionary;
    std::vector<std::string> docs;
    ConverterJson jsonData;
    std::vector<std::string> threads;
    std::vector<InvertedIndex>classParts;
};


#endif //FINALPROJECT_INVERTEDINDEX_H
