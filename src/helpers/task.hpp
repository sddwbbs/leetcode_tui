#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <gumbo.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "requests/dailyTaskRequest.hpp"
#include "requests/runCodeRequests.hpp"

using std::string;
using json = nlohmann::json;
using std::vector;

struct TaskData {
    int id;
    int frontendId;
    string titleSlug;
    string title;
    string difficulty;
    string content;
    json topicTags;
    json codeSnippets;
    bool paidOnly;
};

struct ResultData {
    int totalTestCases;
    int totalCorrect;
    vector<string> codeAnswer;
    vector<string> expectedCodeAnswer;
    string statusMessage;
    string fullCompileError;
    string statusMemory;
    string statusRuntime;
};

class Task {
    pqxx::connection &conn;

    TaskData singleTask;

    ResultData resultData;

    void extractText(GumboNode* node, string& plainText);

    void saveToDb(bool dailyInDb, pqxx::work &tx);

    void readFromDb(pqxx::work &tx);

public:
    Task() = delete;

    Task(Task &) = delete;

    Task &operator=(Task &) = delete;

    explicit Task(pqxx::connection &conn);

    TaskData &getDailyTask();

    TaskData &getSingleTask();

    [[nodiscard]] ResultData &runCode();

    string submitCode();
};
