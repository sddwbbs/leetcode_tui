#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <pqxx/pqxx>
#include <gumbo.h>
#include <iostream>

#include "requests/dailyTaskRequest.hpp"
#include "requests/codeSnippetRequest.hpp"

using std::string;
using json = nlohmann::json;

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

class Task {
    pqxx::connection &conn;

    TaskData singleTask;

    void extractText(GumboNode* node, string& plainText);

    void saveToDb(int counter);

    void readFromDb();

public:
    Task() = delete;

    Task(Task &) = delete;

    Task &operator=(Task &) = delete;

    explicit Task(pqxx::connection &conn);

    TaskData &getDailyTask();

    TaskData &getSingleTask();

    string runCode();

    string submitCode();
};
