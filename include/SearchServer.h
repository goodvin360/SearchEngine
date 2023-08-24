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
    /*
     * конструктор по умолчанию при вызове принимает на вход ссылку на экземпляр класса InvertedIndex и
     * ссылку на экземпляр класса ConverterJson, инициализирует переменные index и dataJson
     * для последующего доступа через них к объектам и методам классов InvertedIndex и ConverterJson
    */

    std::vector<std::vector<std::pair<int, float>>> searchFoo (const std::vector<std::string> &queries_input);
    /*
     * основной метод класса, формирует результат для последующей записи в файл 'answers.json', осуществляя обработку
     * данных по запросам путем анализа информации о частотном словаре.
     * Возвращает вектор с результатами обработки запросов, размер данного вектора равен числу запросов.
     * Каждый элемент вектора - также вектор, содержащий информацию о резульататах запроса для
     * всех найденных документов с ненулевой релевантностью по данному запросу
    */

private:
    InvertedIndex index;
    ConverterJson dataJson;
    std::map<std::string,int> uniqRequests;
    std::multimap<int,int> preRelevance;
    std::vector<std::vector<std::pair<int, float>>> result;
    std::vector<std::pair<int, float>> relativeIndex;
    void uniqRequestsFill(const std::string& request, int reqNum);
    /*
     * метод для заполнения переменной uniqRequests, представляющей собой ассоциативный массив, ключами которого
     * являются слова из запроса. Выбран тип данных map для того, чтобы сразу формировались данные только о уникальных
     * словах из запроса (отбросить повторы слов в запросе). Значения карты в данном случае не играют роли и указаны
     * равными нулю.
    */

    void preRelevanceFill();
    /*
     * метод первичной обработки запроса, осуществляет заполнение объекта std::multimap<int,int> preRelevance
     * в котором накапливаются данные о абсолютной релевантности каждого слова в запросе по отношению к рассматриваемым
     * документам
    */

    int findMaxAbsRel();
    /*
     * метод для вычисления максимальной абсолютной релевантности, возвращает значение переменной int maxAbsRelevance,
     * используемое в дальнейшем для вычисления относительной релевантности
    */

    int maxAbsRelevance = 0;
    std::vector<std::pair<int, float>> sortRelativeIndex();
    /*
     * метод, осуществляющий сортировку результатов обработки запроса. Сортировка происходит в порядке убывания значения
     * относительной релевантности, при этом если в ряде документов относительная релевантность имеет одинаковое значение,
     * то документы выводятся в результат в порядке возрастания их ID
    */


};

#endif //FINALPROJECT_SEARCHSERVER_H
