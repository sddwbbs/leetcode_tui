#include <cpr/cpr.h>

#include "runCodeRequests.hpp"

json RunCodeRequests::getResult(const int questionId, const string &titleSlug, const string &typedCode) {
    string testCases;
    string runcodeId;
    string lang = "cpp";
    json jsonResponse;
    json jsonRequestBody;

    //Get TestCases
    jsonRequestBody["query"] = R"(
        query consolePanelConfig($titleSlug: String!) {
          question(titleSlug: $titleSlug) {
            questionId
            questionFrontendId
            questionTitle
            enableDebugger
            enableRunCode
            enableSubmit
            enableTestMode
            exampleTestcaseList
            metaData
          }
        }
    )";
    jsonRequestBody["variables"]["titleSlug"] = titleSlug;
    jsonRequestBody["operationName"] = "consolePanelConfig";
    auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                              cpr::Header{{"Content-Type", "application/json"}},
                              cpr::Body{jsonRequestBody.dump()});
    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        for (auto it = jsonResponse["data"]["question"]["exampleTestcaseList"].begin(); it != jsonResponse["data"]["question"]["exampleTestcaseList"].end(); ++it) {
            testCases += *it;
            if (std::next(it) != jsonResponse["data"]["question"]["exampleTestcaseList"].end()) {
                testCases += '\n';
            }
        }
    } else {
        return "";
    }
    jsonRequestBody.clear();

    //Get runcode id
    jsonRequestBody["lang"] = lang;
    jsonRequestBody["question_id"] = questionId;
    jsonRequestBody["typed_code"] = typedCode;
    jsonRequestBody["data_input"] = testCases;
    response = cpr::Post(cpr::Url{"https://leetcode.com/problems/" + titleSlug + "/interpret_solution/"},
                                cpr::Header{{"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNDE0ODkyOSwiaXAiOiI3OS4xMDQuNi4yOSIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.ANSqE_UclnEeSwv5E7y8EAzLSABJ6IMTLnJq2LO8w3s"},
                                            {"Content-Type", "application/json"},
                                            {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"},
                                            {"Referer", "https://leetcode.com/problems/" + titleSlug + "/"}},
                                cpr::Body{jsonRequestBody.dump()});
    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        runcodeId = jsonResponse["interpret_id"];
    } else {
        return "";
    }

    //Check running task status
    while (true) {
        response = cpr::Get(cpr::Url{"https://leetcode.com/submissions/detail/" + runcodeId + "/check/"},
                            cpr::Header{{"Content-Type", "application/json"},
                                                    {"Cookie", "csrftoken=CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV; LEETCODE_SESSION=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJfYXV0aF91c2VyX2lkIjoiNzQxOTYzNiIsIl9hdXRoX3VzZXJfYmFja2VuZCI6ImFsbGF1dGguYWNjb3VudC5hdXRoX2JhY2tlbmRzLkF1dGhlbnRpY2F0aW9uQmFja2VuZCIsIl9hdXRoX3VzZXJfaGFzaCI6ImJlZTA3MmJkNzc3OGZhYzQzYzEzOTY0ZjkyOTBjNTdiMWM1ZGMzZThjODYwNWIzNmJlNDIxNTc0MjZkODI4MDciLCJpZCI6NzQxOTYzNiwiZW1haWwiOiJkLmZyb20yMDE2QHlhbmRleC5ydSIsInVzZXJuYW1lIjoic2RXYnMiLCJ1c2VyX3NsdWciOiJzZFdicyIsImF2YXRhciI6Imh0dHBzOi8vYXNzZXRzLmxlZXRjb2RlLmNvbS91c2Vycy9hdmF0YXJzL2F2YXRhcl8xNjk1NzQ3NjkyLnBuZyIsInJlZnJlc2hlZF9hdCI6MTcxNDE0ODkyOSwiaXAiOiI3OS4xMDQuNi4yOSIsImlkZW50aXR5IjoiZDE1NmNmYjZmZWQ5MGRmZWVkMTg4MzlmYjkzMjk2NGMiLCJzZXNzaW9uX2lkIjo1NDg3MzYwNCwiX3Nlc3Npb25fZXhwaXJ5IjoxMjA5NjAwfQ.ANSqE_UclnEeSwv5E7y8EAzLSABJ6IMTLnJq2LO8w3s"},
                                                    {"X-Csrftoken", "CVzqvFMWhiR5gBYpA81AkgCzKoCrTr4mKznRE69iy6c3feGEYH3nNCosU8P0LLrV"}});
        if (response.status_code == 200) {
            jsonResponse = json::parse(response.text);
            if (jsonResponse["state"] == "SUCCESS") break;
        }
        usleep(100);
    }

    return jsonResponse;
}

