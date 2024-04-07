#include "dailyquestionrequest.hpp"

string DailyQuestionRequest::titleSlug;
string DailyQuestionRequest::title;
string DailyQuestionRequest::htmlContent;
string DailyQuestionRequest::content;
time_t DailyQuestionRequest::refreshTime;

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
    htmlContent = jsonResponse["data"]["question"]["content"];
    return true;
}

bool DailyQuestionRequest::saveToFile() {
    std::ofstream myFileOutput;
    myFileOutput.open("content.html");
    myFileOutput << refreshTime << "<p>" << title << htmlContent;
    myFileOutput.close();

    system("elinks -dump content.html > content.txt");
    std::ifstream myFileInput ("content.txt");
    string date;
    if (myFileInput.is_open()) {
        std::getline(myFileInput, date);
        std::stringstream buffer;
        std::getline(myFileInput >> std::ws, title);
        buffer << title << myFileInput.rdbuf();
        content = buffer.str();
    } else return false;
    myFileInput.close();
    return true;
}

string DailyQuestionRequest::getQuestion() {
    std::time_t curTime = std::time(nullptr);

    refreshTime = curTime;
    std::tm* refreshDate = std::localtime(&refreshTime);
    refreshDate->tm_hour = 7;
    refreshDate->tm_min = 0;
    refreshTime = std::mktime(refreshDate);

    string firstLine;
    bool fileOpened = false;
    time_t fileRefreshTime = refreshTime;
    std::ifstream myFileInput("content.txt");
    if (myFileInput.is_open()) {
        fileOpened = true;
        std::getline(myFileInput >> std::ws, firstLine);
        std::stringstream buffer;
        std::getline(myFileInput >> std::ws, title);
        buffer << title << myFileInput.rdbuf();
        content = buffer.str();
        fileRefreshTime = std::stoi(firstLine);
        if (fileRefreshTime > refreshTime) refreshTime = fileRefreshTime;
        myFileInput.close();
    }

    if (!fileOpened || curTime >= refreshTime || curTime >= fileRefreshTime) {
        refreshDate->tm_mday += 1;
        refreshTime = std::mktime(refreshDate);
        while (!getTitleSlug()) getTitleSlug();
        while (!getContent()) getContent();
        if (saveToFile()) return content;
        else return "Error while saving file";
    }

    return content;
}

