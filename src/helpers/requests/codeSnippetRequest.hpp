#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <curl/curl.h>

using json = nlohmann::json;
using std::string;

class CodeSnippetRequest {
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
public:
    static json getCodeSnippet(string &titleSlug, string &language) {
        CURL *curl;
        CURLcode res;
        string readBuffer;
        json jsonResponse;
        std::string payload = R"({"query":"\n    query questionEditorData($titleSlug: String!) {\n  question(titleSlug: $titleSlug) {\n    questionId\n    questionFrontendId\n    codeSnippets {\n      lang\n      langSlug\n      code\n    }\n    envInfo\n    enableRunCode\n    hasFrontendPreview\n    frontendPreviews\n  }\n}\n    ","variables":{"titleSlug":")" + titleSlug + R"("},"operationName":"questionEditorData"})";

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://leetcode.com/graphql/");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                return false;
            else {
                try {
                    jsonResponse = json::parse(readBuffer);
                } catch (const std::exception& e) {
                    return false;
                }
            }
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        } else {
            return false;
        }

        curl_global_cleanup();
        return jsonResponse;
    }
};