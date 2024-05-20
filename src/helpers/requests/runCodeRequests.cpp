#include <cpr/cpr.h>

#include "runCodeRequests.hpp"

json RunCodeRequests::getResult(const int questionId, const string &titleSlug, const string &typedCode) {
    string testCases;
    string runcodeId;
    string lang = "cpp";
    json jsonResponse;
    json jsonRequestBody;
    string cookie = "csrftoken=" + Config::getCsrftoken() + "; LEETCODE_SESSION=" + Config::getLeetcodeSession();

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
                                cpr::Header{{"Cookie", cookie.c_str()},
                                            {"Content-Type", "application/json"},
                                            {"X-Csrftoken", Config::getCsrftoken().c_str()},
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
                                                    {"Cookie", cookie.c_str()},
                                                    {"X-Csrftoken", Config::getCsrftoken().c_str()}});
        if (response.status_code == 200) {
            jsonResponse = json::parse(response.text);
            if (jsonResponse["state"] == "SUCCESS") break;
        }
        usleep(100);
    }

    return jsonResponse;
}

