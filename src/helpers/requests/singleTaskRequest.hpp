#pragma once

#include "../config.hpp"

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using std::string;

class SingleTaskRequest {

public:
    static json getAllData(string &titleSlug);
};