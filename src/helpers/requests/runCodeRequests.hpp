#pragma once

#include "../config.hpp"

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using std::string;

class RunCodeRequests {

public:
    static json getResult(int questionId, const string &titleSlug, const string &typedCode);
};