#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using std::string;

class DailyTaskRequest {

public:
    static json getAllData();
};