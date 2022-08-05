#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"
#include <vector>
#include <exception>
#include <iomanip>
#include <sstream>

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
        nlohmann::json final_json, answers_json, request_json;
        auto relevance_json = nlohmann::json::array();
        int request_count=1, docid_count=0;
        std::string request_count_string;
        std::stringstream request_ss;

        for(auto request = answers.begin(); request != answers.end(); request++)
        {
            /**Check if there any relevances at all*/
            for(auto docid= request->begin(); docid != request->end(); docid++)
            {
                if(docid->second>0)
                {
                    request_json["result"]="true";
                    break;
                }
            }

            if(request_json.find("result")==request_json.end())
            {
                request_json["result"] = "false";
            }
            else
            {
                for(auto docid=request->begin(); docid!=request->end(); docid++)
                {
                    if(docid->second > 0 && docid_count == 0)
                    {
                        request_json["docid"] = docid->first;
                        request_json["rank"] = docid->second;
                        docid_count++;
                    }
                    else if(docid->second > 0 && docid_count == 1)
                    {
                        /** add docid and rank that already in config into array*/
                        relevance_json.push_back(
                                        nlohmann::json ()={{"docid", request_json["docid"]},{"rank", request_json["rank"]}}
                                );
                        /** erase those docid and rank */
                        request_json.erase("docid");
                        request_json.erase("rank");

                        relevance_json.push_back(
                                nlohmann::json ()={{"docid", docid->first},{"rank", docid->second}}
                                );

                        docid_count++;
                    }
                    else if(docid->second > 0 && docid_count > 1)
                    {
                        relevance_json.push_back(
                                nlohmann::json ()={{"docid", docid->first},{"rank", docid->second}}
                        );
                    }
                    if(docid_count>1)
                        request_json["relevance"] = relevance_json;
                }

                docid_count = 0;
                relevance_json.clear();
            }
            request_ss << std::setw(3) << std::setfill('0') << request_count;
            request_count_string = request_ss.str();
            request_ss.str(std::string());

            answers_json["request" + request_count_string] = request_json;
            request_json.clear();
            request_count++;
        }



        final_json["answers"]=answers_json;
        std::ofstream json_file("answers.json");

        json_file << final_json;
        json_file.close();
    }
};

int main()
{
    ConverterJSON someStuff;

    std::vector<std::vector<std::pair<int, float>>> answers;

    std::vector<std::pair<int, float>> a, b, c;
    a.push_back(std::pair<int, float> (0, 0.892));
    a.push_back(std::pair<int, float> (1, 0.452));
    a.push_back(std::pair<int, float> (2, 0.231));

    b.push_back(std::pair<int, float> (0, 0.777));

    answers.push_back(a);
    answers.push_back(c);
    answers.push_back(b);

    someStuff.putAnswers(answers);
    return 0;
}
