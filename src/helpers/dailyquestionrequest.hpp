#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <curl/curl.h>
#include <ctime>
#include <regex>

using json = nlohmann::json;
using std::string;

class DailyQuestionRequest {
    static string titleSlug;
    static string title;
    static string content;
    static int curDay;

    static size_t writeCallback(void *, size_t, size_t, void *);
    static bool getTitleSlug();
    static bool getContent();
    static void removeHtmlTags();
public:
    static string getQuestion();
};
