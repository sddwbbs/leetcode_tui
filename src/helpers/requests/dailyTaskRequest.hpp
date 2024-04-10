#pragma once

#include "../task.hpp"
#include <nlohmann/json.hpp>
#include <string>
#include <curl/curl.h>
#include <ctime>
#include <fstream>

using json = nlohmann::json;
using std::string;

struct TaskData;

class DailyTaskRequest {
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
public:
    static json getTitleSlug();
    static json getContent(string &titleSlug);
    static bool saveToFile(TaskData &dailyTask, time_t &dailyRefreshTime);
};
