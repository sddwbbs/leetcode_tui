#pragma once

#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <curl/curl.h>

using json = nlohmann::json;
using std::string;

const char* url = "https://leetcode.com/graphql/";

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string removeHtmlTags(const std::string& htmlStr) {
    // Replace HTML entities with ASCII characters
    std::string replacedStr = htmlStr;
    size_t pos = 0;
    while ((pos = replacedStr.find("\n\n")) != std::string::npos) {
        replacedStr.replace(pos, 4, "++");
    }
    while ((pos = replacedStr.find("&quot;")) != std::string::npos) {
        replacedStr.replace(pos, 6, "\""); // Replace "&quot;" with double quote
    }
    while ((pos = replacedStr.find("&lt;")) != std::string::npos) {
        replacedStr.replace(pos, 4, "<"); // Replace "&lt;" with less-than sign
    }
    while ((pos = replacedStr.find("&nbsp;")) != std::string::npos) {
        replacedStr.replace(pos, 6, "");
    }

    // Remove HTML tags
    std::regex regex("<[^>]*>");
    return std::regex_replace(replacedStr, regex, "");
}

class DailyRequest {
public:
    static string getResponse() {
        CURL *curl;
        CURLcode res;
        string readBuffer;
        json jsonResponse;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"query\":\"\\n    query questionOfToday {\\n  activeDailyCodingChallengeQuestion {\\n    date\\n    userStatus\\n    link\\n    question {\\n      acRate\\n      difficulty\\n      freqBar\\n      frontendQuestionId: questionFrontendId\\n      isFavor\\n      paidOnly: isPaidOnly\\n      status\\n      title\\n      titleSlug\\n      hasVideoSolution\\n      hasSolution\\n      topicTags {\\n        name\\n        id\\n        slug\\n      }\\n    }\\n  }\\n}\\n    \",\"variables\":{},\"operationName\":\"questionOfToday\"}");

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                return "Failed to perform HTTP request";
            else {
                try {
                    jsonResponse = json::parse(readBuffer);
                } catch (const std::exception& e) {
                    return "Error";
                }
            }
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        } else {
            return "Failed to initialize libcurl";
        }

        curl_global_cleanup();
        string task = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["link"];

        return task;
    }
};
