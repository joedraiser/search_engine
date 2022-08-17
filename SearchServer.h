//
// Created by Evgenii.Korchagov on 14.08.2022.
//

#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include <cstddef>
#include "InvertedIndex.h"
#include "ConverterJSON.h"

class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx) : _index(idx){ };
/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
private:
    InvertedIndex _index;
};

std::vector<Entry>::iterator findDocid(std::vector<Entry>& array, int doc_id);
int getMaxAbsRelevancy(const std::vector<std::string>& docs);
void fillArrayTotalEntriesForQuery(const std::string& query, std::vector<Entry>& arrayToFill, InvertedIndex invIndex);

#endif //SEARCH_ENGINE_SEARCHSERVER_H
