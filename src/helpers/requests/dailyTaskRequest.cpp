#include "dailyTaskRequest.hpp"

#include <cpr/cpr.h>

json DailyTaskRequest::getAllData() {
    json result;
    json jsonResponse;

    auto response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                              cpr::Header{{"Content-Type", "application/json"}},
                              cpr::Body{"{\n"
                                        "\t\"query\": \"\\n    query questionOfToday {\\n  activeDailyCodingChallengeQuestion {\\n      link\\n    question {\\n      acRate\\n      difficulty\\n      freqBar\\n      frontendQuestionId: questionFrontendId\\n      isFavor\\n      paidOnly: isPaidOnly\\n      status\\n      title\\n      titleSlug\\n          topicTags {\\n        name\\n        id\\n        slug\\n      }\\n    }\\n  }\\n}\\n    \",\n"
                                        "\t\"variables\": {},\n"
                                        "\t\"operationName\": \"questionOfToday\"\n"
                                        "}"});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["difficulty"] = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["difficulty"];
        result["title"] = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["title"];
        result["titleSlug"] = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["titleSlug"];
        result["topicTags"] = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["topicTags"];
        result["frontendId"] = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["frontendQuestionId"];
        result["paidOnly"] = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["paidOnly"];
    } else {
        return json::parse("");
    }

    string titleSlug = result["titleSlug"];
    string requestBody = "{\n"
                         "\t\"query\": \"\\n    query questionContent($titleSlug: String!) {\\n  question(titleSlug: $titleSlug) {\\n    content\\n    mysqlSchemas\\n    dataSchemas\\n  }\\n}\\n    \",\n"
                         "\t\"variables\": {\n"
                         "\t\t\"titleSlug\": \"" + titleSlug + "\"\n"
                         "\t},\n"
                         "\t\"operationName\": \"questionContent\"\n"
                         "}";

    response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                         cpr::Header{{"Content-Type", "application/json"}},
                         cpr::Body{requestBody});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["content"] = jsonResponse["data"]["question"]["content"];
    } else {
        return json::parse("");
    }

    requestBody = "{\n"
                  "\t\"query\": \"\\n    query questionEditorData($titleSlug: String!) {\\n  question(titleSlug: $titleSlug) {\\n    questionId\\n    questionFrontendId\\n    codeSnippets {\\n      lang\\n      langSlug\\n      code\\n    }\\n    envInfo\\n    enableRunCode\\n    hasFrontendPreview\\n    frontendPreviews\\n  }\\n}\\n    \",\n"
                  "\t\"variables\": {\n"
                  "\t\t\"titleSlug\": \"" + titleSlug + "\"\n"
                  "\t},\n"
                  "\t\"operationName\": \"questionEditorData\"\n"
                  "}";

    response = cpr::Post(cpr::Url{"https://leetcode.com/graphql/"},
                         cpr::Header{{"Content-Type", "application/json"}},
                         cpr::Body{requestBody});

    if (response.status_code == 200) {
        jsonResponse = json::parse(response.text);
        result["id"] = jsonResponse["data"]["question"]["questionId"];
        result["codeSnippets"] = jsonResponse["data"]["question"]["codeSnippets"];
    } else {
        return json::parse("");
    }

    return result;
}