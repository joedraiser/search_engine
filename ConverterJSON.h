#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <vector>
#include <exception>
#include <iomanip>

class empty_config: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Config file is empty";
    }
};

class missing_config: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Config file is missing";
    }
};

class ConverterJSON
{
public:
    ConverterJSON() = default;
/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments();

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit();
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();

/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
