#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using std::string;

class RunCodeRequests {

public:
    static string getResult(int questionId, const string &titleSlug, const string &typedCode);
};