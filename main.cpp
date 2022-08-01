#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <vector>
#include <exception>

std::string CONFIG_FILE=".//config.json";

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
    std::vector<std::string> GetTextDocuments()
    {
        try {
            std::vector<std::string> textDocuments;
            std::ifstream temp_strm(CONFIG_FILE);
            if (!temp_strm.is_open())
                throw missing_config();
            nlohmann::json config_json;
            temp_strm >> config_json;
            temp_strm.close();
            std::stringstream buff;

            for (auto fileNames_it = config_json["files"].begin();
            fileNames_it != config_json["files"].end();
            fileNames_it++)
            {
                temp_strm.open(fileNames_it.value());
                buff << temp_strm.rdbuf();
                textDocuments.push_back(buff.str());
                temp_strm.close();
                std::stringstream().swap(buff);
            }

            return textDocuments;
        }
        catch (std::exception &e)
        {
            std::cout << e.what();
        }
    }
/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return
*/
    int GetResponsesLimit()
    {
        try {
            std::vector<std::string> textDocuments;
            std::ifstream temp_strm(CONFIG_FILE);
            if(!temp_strm.is_open())
                throw missing_config();

            nlohmann::json config_json;
            temp_strm >> config_json;
            if (config_json.find("config")==config_json.end()) {
                throw empty_config();
            }

            return (int) config_json["config"]["max_responses"];
        }
        catch (std::exception &e)
        {
            std::cout << e.what();
        }
    }
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests()
    {
        std::vector<std::string> requests;
        std::ifstream temp_strm(".//requests.json");
        nlohmann::json requests_json;
        temp_strm >> requests_json;

        for(auto requests_it=requests_json["requests"].begin(); requests_it != requests_json["requests"].end(); requests_it++)
        {
            requests.push_back(requests_it.value());
        }

        return requests;
    }
/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
    {
        nlohmann::json answers_json, request_json, relevance_json;
        int request_count=1, docid_count=0;

        for(auto requests = answers.begin(); requests!=answers.end(); requests++)
        {
            for(auto docid=requests->begin(); docid!=requests->end(); docid++)
            {
                if(docid->second>0)
                {
                    request_json["result"]="true";
                    break;
                }
            }

            if(!request_json.contains("result"))
            {
                request_json["result"] = "false";
                continue;
            }
            else
            {
                for(auto docid=requests->begin(); docid!=requests->end(); docid++)
                {
                    if(docid->second > 0)
                    {
                        if(docid_count==0)
                        {
                            request_json["docid"] = docid->first;
                            request_json["rank"] = docid->second;
                            docid_count++;
                        }
                        else
                        {

                        }
                    }
                }
            }
        }
    }
};

int main()
{
    ConverterJSON someStuff;

    auto requests= someStuff.GetResponsesLimit();
    return 0;
}
