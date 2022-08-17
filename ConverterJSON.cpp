#include "ConverterJSON.h"
std::string CONFIG_FILE=".//config.json";

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    {
        try {
            std::vector<std::string> textDocuments;
            std::ifstream temp_strm(CONFIG_FILE);

/** check if config file opened */
            if (!temp_strm.is_open())
                throw missing_config();
/** read config file */
            nlohmann::json config_json;
            temp_strm >> config_json;
            temp_strm.close();

            std::stringstream buff;
            std::string word_to_check;
            int word_count=1;

/** iterate through resources files */
            for (auto fileNames_it = config_json["files"].begin();
                 fileNames_it != config_json["files"].end();
                 fileNames_it++
                    )
            {
/** open resource file */
                temp_strm.open(fileNames_it.value());
                if(!temp_strm.is_open())
                {
                    std::cout << fileNames_it.value() << " is missing\n";
                    continue;
                }

                buff << temp_strm.rdbuf();

/** check 1000< words, 100< word length */
                while(!buff.eof())
                {
                    buff >> word_to_check;
                    if(word_to_check.length()>100)
                    {
                        std::cout << fileNames_it.value() << " contains very lengthy word\n";
                        std::stringstream().swap(buff);
                        break;
                    }
                    else if(word_count > 1000)
                    {
                        std::cout << fileNames_it.value() << " contains too much words\n";
                        std::stringstream().swap(buff);
                        break;
                    }
                    word_count++;
                }
                word_count=1;

                textDocuments.push_back(buff.str());
                temp_strm.close();
/** crear the buffer */
                std::stringstream().swap(buff);
            }

            return textDocuments;
        }
        catch (std::exception &e)
        {
            std::cout << e.what();
            return std::vector<std::string> ();
        }
    }
}

int ConverterJSON::GetResponsesLimit() {
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
            return 0;
        }
    }
}

std::vector<std::string> ConverterJSON::GetRequests() {
    {
        std::vector<std::string> requests;
        std::ifstream temp_strm(".//requests.json");
        nlohmann::json requests_json;
        temp_strm >> requests_json;

        std::stringstream ss;
        int word_count=0, request_count=0;

        /** iterate through each request */
        for(auto requests_it=requests_json["requests"].begin(); requests_it != requests_json["requests"].end(); requests_it++)
        {
            if(request_count>999)
            {
                std::cout << "Too much requests\n";
                break;
            }

            ss << requests_it.value();
            /** count words in request */
            word_count=std::distance(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>());
            /** validate words quantity */
            if(word_count>10 || word_count<1)
            {
                std::cout << "Request No " << request_count << ": Number of requests is not valid. There are more than 10 requests or less than 1\n";
                requests.push_back("");
            }
            else
                requests.push_back(requests_it.value());

            request_count++;
            std::stringstream().swap(ss);
        }

        return requests;
    }
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    {
        nlohmann::json final_json, answers_json, request_json;
        auto relevance_json = nlohmann::json::array();
        int request_count=1, docid_count=0;
        std::string request_count_string;
        std::stringstream request_ss;

        /** iterate throgh requests*/
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
                /** so there are relevances. Add them to request */
            else
            {
                /** iterate through docids*/
                for(auto docid=request->begin(); docid!=request->end(); docid++)
                {
                    /** if there is only one relevance */
                    if(docid->second > 0 && docid_count == 0)
                    {
                        request_json["docid"] = docid->first;
                        request_json["rank"] = docid->second;
                        docid_count++;
                    }
                        /** if there is two relevances */
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
                        /** if there are many relevances */
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
            /** forming request names like Request001, Request002 etc */
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
}

void ConverterJSON::putAnswers(std::vector< std::vector<RelativeIndex> > answers)
{
    {
        nlohmann::json final_json, answers_json, request_json;
        auto relevance_json = nlohmann::json::array();
        int request_count=1, docid_count=0;
        std::string request_count_string;
        std::stringstream request_ss;

        /** iterate throgh requests*/
        for(auto request = answers.begin(); request != answers.end(); request++)
        {
            /**Check if there any relevances at all*/
            for(auto docid= request->begin(); docid != request->end(); docid++)
            {
                if(docid->rank>0)
                {
                    request_json["result"]="true";
                    break;
                }
            }

            if(request_json.find("result")==request_json.end())
            {
                request_json["result"] = "false";
            }
                /** so there are relevances. Add them to request */
            else
            {
                /** iterate through docids*/
                for(auto docid=request->begin(); docid!=request->end(); docid++)
                {
                    /** if there is only one relevance */
                    if(docid->rank > 0 && docid_count == 0)
                    {
                        request_json["docid"] = docid->doc_id;
                        request_json["rank"] = docid->rank;
                        docid_count++;
                    }
                        /** if there is two relevances */
                    else if(docid->rank > 0 && docid_count == 1)
                    {
                        /** add docid and rank that already in config into array*/
                        relevance_json.push_back(
                                nlohmann::json ()={{"docid", request_json["docid"]},{"rank", request_json["rank"]}}
                        );
                        /** erase those docid and rank */
                        request_json.erase("docid");
                        request_json.erase("rank");

                        relevance_json.push_back(
                                nlohmann::json ()={{"docid", docid->doc_id},{"rank", docid->rank}}
                        );

                        docid_count++;
                    }
                        /** if there are many relevances */
                    else if(docid->rank > 0 && docid_count > 1)
                    {
                        relevance_json.push_back(
                                nlohmann::json ()={{"docid", docid->doc_id},{"rank", docid->rank}}
                        );
                    }
                    if(docid_count>1)
                        request_json["relevance"] = relevance_json;
                }

                docid_count = 0;
                relevance_json.clear();
            }
            /** forming request names like Request001, Request002 etc */
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
}
