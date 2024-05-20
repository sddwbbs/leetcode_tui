#include "questionListRequest.hpp"

#include <cpr/cpr.h>

vector<json> QuestionListRequest::getQuestionList(string &searchText) {
    json jsonResponse;
    json requestBody;
    vector<json> result;
    string cookie = "csrftoken=" + Config::getCsrftoken() + "; LEETCODE_SESSION=" + Config::getLeetcodeSession();

    requestBody["query"] = "query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {  problemsetQuestionList: questionList(    categorySlug: $categorySlug    limit: $limit    skip: $skip    filters: $filters  ) {    total: totalNum    questions: data {      acRate      difficulty      freqBar      frontendQuestionId: questionFrontendId      isFavor      paidOnly: isPaidOnly      status      title      titleSlug      topicTags {        name        id        slug      }      hasSolution      hasVideoSolution    }  }}    ";
    requestBody["variables"]["categorySlug"] = "all-code-essentials";
    requestBody["variables"]["skip"] = 0;
    requestBody["variables"]["limit"] = 50;
    if (!searchText.empty())
        requestBody["variables"]["filters"]["searchKeywords"] = searchText;
    else
        requestBody["variables"]["filters"] = json::object();
    requestBody["operationName"] = "problemsetQuestionList";

    auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                         cpr::Header{{"Content-Type", "application/json"},
                                     {"Referer", "https://leetcode.com/problemset/?page=1&search=" + searchText},
                                     {"Cookie", cookie.c_str()},
                                     {"X-Csrftoken", Config::getCsrftoken().c_str()}},
                         cpr::Body{requestBody.dump()});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result = jsonResponse["data"]["problemsetQuestionList"]["questions"];
    } else {
        return json::parse("");
    }

    return result;
}