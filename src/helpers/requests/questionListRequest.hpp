#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;
using std::string;
using std::vector;

class QuestionListRequest {
public:
    static vector<json> getQuestionList(string &searchText);
};