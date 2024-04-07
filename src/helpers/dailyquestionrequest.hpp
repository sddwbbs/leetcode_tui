#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <curl/curl.h>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <regex>

using json = nlohmann::json;
using std::string;

class DailyQuestionRequest {
    static string titleSlug;
    static string title;
    static string htmlContent;
    static string content;
    static time_t refreshTime;

    static size_t writeCallback(void *, size_t, size_t, void *);
    static bool getTitleSlug();
    static bool getContent();
    static bool saveToFile();
public:
    static string getQuestion();
};
