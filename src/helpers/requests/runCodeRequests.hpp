#pragma once

#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;
using std::string;

class RunCodeRequests {
    string getTestCases(const string &titleSlug);

    string run(const string &titleSlug, const string &testCases);

    string check(const string &runcodeId);

public:
    static string getResult();
};