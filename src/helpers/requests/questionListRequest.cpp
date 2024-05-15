#include "questionListRequest.hpp"

#include <cpr/cpr.h>

vector<json> QuestionListRequest::getQuestionList(string &searchText) {
    json jsonResponse;
    json requestBody;
    vector<json> result;

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
                                     {"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNTM2NDcxNCwiaXAiOiIyMTIuMzQuMTE5LjI1NCIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.R0rI_S1nyv0NWhPTE1XKROsupa-XR_eq4hdoXKINSNQ"},
                                     {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"}},
                         cpr::Body{requestBody.dump()});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result = jsonResponse["data"]["problemsetQuestionList"]["questions"];
    } else {
        return json::parse("");
    }

    return result;
}