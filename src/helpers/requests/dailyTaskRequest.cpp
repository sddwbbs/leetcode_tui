#include "dailyTaskRequest.hpp"

json DailyTaskRequest::getTitleSlug() {
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
    return jsonResponse;
}

json DailyTaskRequest::getContent(string &titleSlug) {
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
    return jsonResponse;
}

bool DailyTaskRequest::saveToFile(TaskData &dailyTask, time_t &dailyRefreshTime) {
    std::ofstream myFileOutput;
    myFileOutput.open("content.html");
    myFileOutput << dailyRefreshTime<< "<p>" << dailyTask.title << dailyTask.htmlContent;
    myFileOutput.close();

    system("elinks -dump content.html > content.txt");
    std::ifstream myFileInput ("content.txt");
    string date;
    if (myFileInput.is_open()) {
        std::getline(myFileInput, date);
        std::stringstream buffer;
        std::getline(myFileInput >> std::ws, dailyTask.title);
        buffer << dailyTask.title << myFileInput.rdbuf();
        dailyTask.content = buffer.str();
    } else return false;
    myFileInput.close();
    return true;
}
