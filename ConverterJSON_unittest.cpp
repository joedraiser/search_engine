#include "ConverterJSON.h"

#include "gtest/gtest.h"
TEST(TestCaseConverterJSON, basic_GetTextDocuments_test)
{
    nlohmann::json config_json;
    config_json["config"] = {{"name", "SkillboxSearchEngine"}, {"version", "0.1"}, {"max_responses", 5}};
    config_json["files"] = {".//resources_test//file001.txt",
                            ".//resources_test//file002.txt",
                            ".//resources_test//file003.txt",
                            ".//resources_test//file004.txt"};

    std::ofstream json_file("config.json");

    json_file << config_json;
    json_file.close();

    std::vector<std::string> expected = {"milk water bread fish", "fate fire milk milk world cot bulk      fish", "game cellphone", "special war change nowhere milk"};

    std::vector<std::string> result = ConverterJSON().GetTextDocuments();

    EXPECT_EQ(result, expected);
}

TEST(TestCaseConverterJSON, basic_PutAnswers_test)
{
    std::vector<std::vector<std::pair<int, float>>> answers;

    std::vector<std::pair<int, float>> a, b, c;
    a.push_back(std::pair<int, float> (0, 0.892));
    a.push_back(std::pair<int, float> (1, 0.452));
    a.push_back(std::pair<int, float> (2, 0.231));

    b.push_back(std::pair<int, float> (0, 0.777));

    answers.push_back(a);
    answers.push_back(c);
    answers.push_back(b);

    ConverterJSON().putAnswers(answers);

    std::ifstream temp_strm(".\\answers.json");

    nlohmann::json result = nlohmann::json::parse(temp_strm);
    temp_strm.close();

    nlohmann::json expected;
    expected["answers"]["request001"]["relevance"] = {{{"docid", 0}, {"rank", 0.8920000195503235}},
                                                      {{"docid", 1}, {"rank", 0.4519999921321869}},
                                                      {{"docid", 2}, {"rank", 0.23100000619888306}}
    };
    expected["answers"]["request001"]["result"] = true;

    expected["answers"]["request002"]["result"] = false;

    expected["answers"]["request003"]["docid"] = 0;
    expected["answers"]["request003"]["rank"] = 0.7770000100135803;
    expected["answers"]["request003"]["result"] = true;

    EXPECT_EQ(expected, result);
}