#include "dailyquestionrequest.hpp"

string DailyQuestionRequest::titleSlug;
string DailyQuestionRequest::title;
string DailyQuestionRequest::content;
int DailyQuestionRequest::curDay = 0;

size_t DailyQuestionRequest::writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool DailyQuestionRequest::getTitleSlug() {
    CURL *curl;
    CURLcode res;
    string readBuffer;
    json jsonResponse;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://leetcode.com/graphql/");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"query\":\"\\n    query questionOfToday {\\n  activeDailyCodingChallengeQuestion {\\n    date\\n    userStatus\\n    link\\n    question {\\n      acRate\\n      difficulty\\n      freqBar\\n      frontendQuestionId: questionFrontendId\\n      isFavor\\n      paidOnly: isPaidOnly\\n      status\\n      title\\n      titleSlug\\n      hasVideoSolution\\n      hasSolution\\n      topicTags {\\n        name\\n        id\\n        slug\\n      }\\n    }\\n  }\\n}\\n    \",\"variables\":{},\"operationName\":\"questionOfToday\"}");

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
    titleSlug = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["titleSlug"];
    title = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["title"];
    return true;
}

bool DailyQuestionRequest::getContent() {
    CURL *curl;
    CURLcode res;
    string readBuffer;
    json jsonResponse;
    string payload = "{\"query\":\"\\n    query questionContent($titleSlug: String!) {\\n  question(titleSlug: $titleSlug) {\\n    content\\n    mysqlSchemas\\n    dataSchemas\\n  }\\n}\\n    \",\"variables\":{\"titleSlug\":\"" + titleSlug + "\"},\"operationName\":\"questionContent\"}";

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
    content = jsonResponse["data"]["question"]["content"];
    return true;
}

void DailyQuestionRequest::removeHtmlTags() {
    // Replace HTML entities with ASCII characters
    size_t pos = 0;
    while ((pos = content.find("\n")) != std::string::npos) {
        content.replace(pos, 2, " ");
    }
    while ((pos = content.find("&quot;")) != std::string::npos) {
        content.replace(pos, 6, "\""); // Replace "&quot;" with double quote
    }
    while ((pos = content.find("&lt;")) != std::string::npos) {
        content.replace(pos, 4, "<"); // Replace "&lt;" with less-than sign
    }
    while ((pos = content.find("&nbsp;")) != std::string::npos) {
        content.replace(pos, 6, " ");
    }

    std::regex regex("<[^>]*>");
    content = std::regex_replace(content, regex, "");
}

string DailyQuestionRequest::getQuestion() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    int day = localTime->tm_mday;
    if (day == curDay && !titleSlug.empty()) return content;
    curDay = day;
    while (!getTitleSlug()) getTitleSlug();
    while (!getContent()) getContent();
    removeHtmlTags();
    content = title + "\n" + content;
    return content;
}
