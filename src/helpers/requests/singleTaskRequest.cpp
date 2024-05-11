#include "singleTaskRequest.hpp"

#include <cpr/cpr.h>

json SingleTaskRequest::getAllData(string &titleSlug) {
    json result;
    json jsonResponse;
    json requestBody;

    //get id, title, frontId, paid only, difficulty
    requestBody["operationName"] = "questionTitle";
    requestBody["query"] = "    query questionTitle($titleSlug: String!) {  question(titleSlug: $titleSlug) {    questionId    questionFrontendId    title    titleSlug    isPaidOnly    difficulty    likes    dislikes    categoryTitle  }}    ";
    requestBody["variables"]["titleSlug"] = titleSlug;
    auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                              cpr::Header{{"Content-Type", "application/json"},
                                          {"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNTM2NDcxNCwiaXAiOiIyMTIuMzQuMTE5LjI1NCIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.R0rI_S1nyv0NWhPTE1XKROsupa-XR_eq4hdoXKINSNQ"},
                                          {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"}},
                              cpr::Body{requestBody.dump()});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["id"] = jsonResponse["data"]["question"]["questionId"];
        result["title"] = jsonResponse["data"]["question"]["title"];
        result["frontendId"] = jsonResponse["data"]["question"]["questionFrontendId"];
        result["paidOnly"] = jsonResponse["data"]["question"]["isPaidOnly"];
        result["difficulty"] = jsonResponse["data"]["question"]["difficulty"];
    } else {
        return json::parse("");
    }

    //get topic tags
    requestBody["operationName"] = "singleQuestionTopicTags";
    requestBody["query"] = "    query singleQuestionTopicTags($titleSlug: String!) {  question(titleSlug: $titleSlug) {    topicTags {      name      slug    }  }}    ";
    response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                              cpr::Header{{"Content-Type", "application/json"},
                                          {"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNTM2NDcxNCwiaXAiOiIyMTIuMzQuMTE5LjI1NCIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.R0rI_S1nyv0NWhPTE1XKROsupa-XR_eq4hdoXKINSNQ"},
                                          {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"}},
                              cpr::Body{requestBody.dump()});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["topicTags"] = jsonResponse["data"]["question"]["topicTags"];
    } else {
        return json::parse("");
    }

    //get content
    requestBody["operationName"] = "questionContent";
    requestBody["query"] = "    query questionContent($titleSlug: String!) {  question(titleSlug: $titleSlug) {    content    mysqlSchemas    dataSchemas  }}    ";
    response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                         cpr::Header{{"Content-Type", "application/json"},
                                     {"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNTM2NDcxNCwiaXAiOiIyMTIuMzQuMTE5LjI1NCIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.R0rI_S1nyv0NWhPTE1XKROsupa-XR_eq4hdoXKINSNQ"},
                                     {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"}},
                         cpr::Body{requestBody.dump()});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["content"] = jsonResponse["data"]["question"]["content"];
    } else {
        return json::parse("");
    }

    //get code snippets
    requestBody["operationName"] = "questionEditorData";
    requestBody["query"] = "    query questionEditorData($titleSlug: String!) {  question(titleSlug: $titleSlug) {    questionId    questionFrontendId    codeSnippets {      lang      langSlug      code    }    envInfo    enableRunCode    hasFrontendPreview    frontendPreviews  }}    ";
    response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                         cpr::Header{{"Content-Type", "application/json"},
                                     {"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNTM2NDcxNCwiaXAiOiIyMTIuMzQuMTE5LjI1NCIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.R0rI_S1nyv0NWhPTE1XKROsupa-XR_eq4hdoXKINSNQ"},
                                     {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"}},
                         cpr::Body{requestBody.dump()});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["codeSnippets"] = jsonResponse["data"]["question"]["codeSnippets"];
    } else {
        return json::parse("");
    }

    return result;
}
