#include "singleTaskRequest.hpp"

#include <cpr/cpr.h>

json SingleTaskRequest::getAllData(string &titleSlug) {
    json result;
    json jsonResponse;
    json requestBody;
    string cookie = "csrftoken=" + Config::getCsrftoken() + "; LEETCODE_SESSION=" + Config::getLeetcodeSession();

    //get id, title, frontId, paid only, difficulty
    requestBody["operationName"] = "questionTitle";
    requestBody["query"] = "    query questionTitle($titleSlug: String!) {  question(titleSlug: $titleSlug) {    questionId    questionFrontendId    title    titleSlug    isPaidOnly    difficulty    likes    dislikes    categoryTitle  }}    ";
    requestBody["variables"]["titleSlug"] = titleSlug;

    for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
        auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                                  cpr::Header{{"Content-Type", "application/json"},
                                      {"Cookie", cookie.c_str()},
                                      {"X-Csrftoken", Config::getCsrftoken().c_str()}},
                                  cpr::Body{requestBody.dump()});

        if (response.status_code == 200) {
            jsonResponse = json::parse(response.text);
            result["id"] = jsonResponse["data"]["question"]["questionId"];
            result["title"] = jsonResponse["data"]["question"]["title"];
            result["frontendId"] = jsonResponse["data"]["question"]["questionFrontendId"];
            result["paidOnly"] = jsonResponse["data"]["question"]["isPaidOnly"];
            result["difficulty"] = jsonResponse["data"]["question"]["difficulty"];
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
    }

    //get topic tags
    requestBody["operationName"] = "singleQuestionTopicTags";
    requestBody["query"] = "    query singleQuestionTopicTags($titleSlug: String!) {  question(titleSlug: $titleSlug) {    topicTags {      name      slug    }  }}    ";

    for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
        auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                             cpr::Header{{"Content-Type", "application/json"},
                                 {"Cookie", cookie.c_str()},
                                 {"X-Csrftoken", Config::getCsrftoken().c_str()}},
                             cpr::Body{requestBody.dump()});

        if (response.status_code == 200) {
            jsonResponse = json::parse(response.text);
            result["topicTags"] = jsonResponse["data"]["question"]["topicTags"];
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
    }

    //get content
    requestBody["operationName"] = "questionContent";
    requestBody["query"] = "    query questionContent($titleSlug: String!) {  question(titleSlug: $titleSlug) {    content    mysqlSchemas    dataSchemas  }}    ";

    for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
        auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                             cpr::Header{{"Content-Type", "application/json"},
                                 {"Cookie", cookie.c_str()},
                                 {"X-Csrftoken", Config::getCsrftoken().c_str()}},
                             cpr::Body{requestBody.dump()});

        if (response.status_code == 200) {
            jsonResponse = json::parse(response.text);
            result["content"] = jsonResponse["data"]["question"]["content"];
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
    }

    //get code snippets
    requestBody["operationName"] = "questionEditorData";
    requestBody["query"] = "    query questionEditorData($titleSlug: String!) {  question(titleSlug: $titleSlug) {    questionId    questionFrontendId    codeSnippets {      lang      langSlug      code    }    envInfo    enableRunCode    hasFrontendPreview    frontendPreviews  }}    ";

    for (int attempt = 0; attempt < MAX_RETRIES; ++attempt) {
        auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                             cpr::Header{{"Content-Type", "application/json"},
                                 {"Cookie", cookie.c_str()},
                                 {"X-Csrftoken", Config::getCsrftoken().c_str()}},
                             cpr::Body{requestBody.dump()});

        if (response.status_code == 200) {
            jsonResponse = json::parse(response.text);
            result["codeSnippets"] = jsonResponse["data"]["question"]["codeSnippets"];
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
    }

    return result;
}
