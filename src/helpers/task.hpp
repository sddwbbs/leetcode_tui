#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "requests/dailyTaskRequest.hpp"
//include "singleTaskRequest.hpp"
#include "requests/codeSnippetRequest.hpp"
//include "runCodeRequest
//include "submitCodeRequest

using std::string;
using json = nlohmann::json;

struct TaskData {
    string titleSlug;
    string title;
    string htmlContent;
    string content;
    string htmlCodeSnippet;
    string codeSnippet;
};

class Task {
    time_t dailyRefreshTime;
    TaskData dailyTask;
    TaskData singleTask;

public:
    Task();
    Task(Task&) = delete;
    Task& operator=(Task&) = delete;

    TaskData& getDailyTask();
    TaskData& getSingleTask();
    string getCodeSnippet(string &titleSlug, string language, TaskData& task);
    string runCode();
    string submitCode();
};
