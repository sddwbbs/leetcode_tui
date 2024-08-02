#include "task.hpp"

Task::Task(pqxx::connection &conn)
        : conn(conn) {}

string Task::exec(const char* cmd) const {
    std::array<char, 128> buffer{};
    string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

string Task::htmlToPlainText(const string& html) const {
    const char* tempFile = "/tmp/temp.html";
    std::ofstream out(tempFile);
    out << html;
    out.close();

    string command = "elinks -dump " + string(tempFile);
    string plainText = exec(command.c_str());

    std::remove(tempFile);

    return plainText;
}

void Task::saveToDb(bool dailyInDb, pqxx::work &tx, bool isDaily) {
    if (isDaily) {
        if (dailyInDb)
            try {
                tx.exec("UPDATE tasks SET refresh_time = NULL WHERE refresh_time IS NOT NULL");
            } catch (std::exception const &e) {
                std::cerr << "ERROR: " << e.what() << '\n';
            }

        std::time_t curTime = std::time(nullptr);
        std::tm *timeInfo = std::localtime(&curTime);
        std::tm *refreshDate = timeInfo;

        if (timeInfo->tm_hour >= 0 && timeInfo->tm_hour < 7) {
            refreshDate->tm_hour = 7;
            refreshDate->tm_min = 0;
        } else {
            std::time_t nextDayTime = curTime + 24 * 60 * 60;
            std::tm *nextDayInfo = std::localtime(&nextDayTime);
            refreshDate->tm_mday = nextDayInfo->tm_mday;
            refreshDate->tm_mon = nextDayInfo->tm_mon;
            refreshDate->tm_year = nextDayInfo->tm_year;
            refreshDate->tm_hour = 7;
            refreshDate->tm_min = 0;
        }
        std::time_t refreshedTime = std::mktime(refreshDate);

        bool alreadyInTable = false;
        try {
            alreadyInTable = tx.query_value<bool>(
                    "SELECT COUNT(*) FROM tasks WHERE id = " + std::to_string(singleTask.id));
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << '\n';
        }

        if (alreadyInTable) {
            try {
                tx.exec_params("UPDATE tasks SET refresh_time = $1 WHERE id = $2", refreshedTime,
                               singleTask.id);
            } catch (std::exception const &e) {
                std::cerr << "ERROR: " << e.what() << '\n';
            }
        } else {
            try {
                tx.exec_params(
                        "INSERT INTO tasks (id, frontend_id, title_slug, title, difficulty, content, topic_tags, code_snippets, paid_only, refresh_time) "
                        "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)",
                        singleTask.id, singleTask.frontendId, singleTask.titleSlug, singleTask.title,
                        singleTask.difficulty, singleTask.content, singleTask.topicTags.dump(),
                        singleTask.codeSnippets.dump(), singleTask.paidOnly, refreshedTime);
            } catch (std::exception const &e) {
                std::cerr << "ERROR: " << e.what() << '\n';
            }
        }
    } else {
        try {
            tx.exec_params(
                    "INSERT INTO tasks (id, frontend_id, title_slug, title, difficulty, content, topic_tags, code_snippets, paid_only) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)",
                    singleTask.id, singleTask.frontendId, singleTask.titleSlug, singleTask.title,
                    singleTask.difficulty, singleTask.content, singleTask.topicTags.dump(),
                    singleTask.codeSnippets.dump(), singleTask.paidOnly);
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << '\n';
        }
    }
}

void Task::readFromDb(pqxx::work &tx, bool isDaily) {
    if (isDaily) {
        try {
            pqxx::result result = tx.exec(
                    "SELECT id, frontend_id, title_slug, title, difficulty, content, topic_tags, code_snippets, paid_only "
                    "FROM tasks WHERE refresh_time IS NOT NULL");

            const auto &row = result.begin();
            singleTask.id = row["id"].as<int>();
            singleTask.frontendId = row["frontend_id"].as<int>();
            singleTask.titleSlug = row["title_slug"].as<string>();
            singleTask.title = row["title"].as<string>();
            singleTask.difficulty = row["difficulty"].as<string>();
            singleTask.content = row["content"].as<string>();
            singleTask.topicTags = json::parse(row["topic_tags"].as<string>());
            singleTask.codeSnippets = json::parse(row["code_snippets"].as<string>());
            singleTask.paidOnly = row["paid_only"].as<bool>();
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << '\n';
        }
    } else {
        try {
            pqxx::result result = tx.exec(
                    "SELECT id, frontend_id, title, difficulty, content, topic_tags, code_snippets, paid_only "
                    "FROM tasks WHERE title_slug = '" + singleTask.titleSlug + "'");

            const auto &row = result.begin();
            singleTask.id = row["id"].as<int>();
            singleTask.frontendId = row["frontend_id"].as<int>();
            singleTask.title = row["title"].as<string>();
            singleTask.difficulty = row["difficulty"].as<string>();
            singleTask.content = row["content"].as<string>();
            singleTask.topicTags = json::parse(row["topic_tags"].as<string>());
            singleTask.codeSnippets = json::parse(row["code_snippets"].as<string>());
            singleTask.paidOnly = row["paid_only"].as<bool>();
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << '\n';
        }
    }
}

TaskData &Task::getDailyTask() {
    std::time_t curTime = std::time(nullptr);
    std::tm *timeInfo = std::localtime(&curTime);
    std::tm *refreshDate = timeInfo;

    pqxx::work tx{conn};

    bool dailyInDb = false;
    try {
        dailyInDb = tx.query_value<bool>(
                "SELECT COUNT(*) FROM tasks WHERE refresh_time IS NOT NULL");
    } catch (std::exception const &e) {
        std::cerr << '\n' << "ERROR: " << e.what() << '\n';
    }

    time_t refreshTimeFromDb = 0;
    if (dailyInDb) {
        try {
            refreshTimeFromDb = tx.query_value<time_t>(
                    "SELECT refresh_time FROM tasks WHERE refresh_time IS NOT NULL");
        } catch (std::exception const &e) {
            std::cerr << '\n' << "ERROR: " << e.what() << '\n';
        }
    }

    if (!dailyInDb || curTime >= refreshTimeFromDb) {
        json jsonDailyTask = DailyTaskRequest::getAllData();

        singleTask.id = std::stoi(jsonDailyTask["id"].get<string>());
        singleTask.frontendId = std::stoi(jsonDailyTask["frontendId"].get<string>());
        singleTask.titleSlug = jsonDailyTask["titleSlug"].get<string>();
        singleTask.title = jsonDailyTask["title"].get<string>();
        singleTask.difficulty = jsonDailyTask["difficulty"].get<string>();
        singleTask.content = jsonDailyTask["content"].get<string>();

        singleTask.content = htmlToPlainText(singleTask.content);

        singleTask.topicTags = jsonDailyTask["topicTags"];
        singleTask.codeSnippets = jsonDailyTask["codeSnippets"];
        singleTask.paidOnly = jsonDailyTask["paidOnly"].get<bool>();

        saveToDb(dailyInDb, tx, true);

        tx.commit();

        return singleTask;
    }

    readFromDb(tx, true);

    return singleTask;
}

TaskData &Task::getSingleTask(string &titleSlug) {
    pqxx::work tx{conn};

    singleTask.titleSlug = titleSlug;

    bool alreadyInDb = false;
    try {
        alreadyInDb = tx.query_value<bool>(
                "SELECT COUNT(*) FROM tasks WHERE title_slug = '" + singleTask.titleSlug + "'");
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }

    if (!alreadyInDb) {
        json jsonSingleTask = SingleTaskRequest::getAllData(singleTask.titleSlug);

        singleTask.id = std::stoi(jsonSingleTask["id"].get<string>());
        singleTask.frontendId = std::stoi(jsonSingleTask["frontendId"].get<string>());
        singleTask.title = jsonSingleTask["title"].get<string>();
        singleTask.difficulty = jsonSingleTask["difficulty"].get<string>();
        singleTask.content = jsonSingleTask["content"].get<string>();

        singleTask.content = htmlToPlainText(singleTask.content);

        singleTask.topicTags = jsonSingleTask["topicTags"];
        singleTask.codeSnippets = jsonSingleTask["codeSnippets"];
        singleTask.paidOnly = jsonSingleTask["paidOnly"].get<bool>();

        saveToDb(false, tx, false);

        tx.commit();

        return singleTask;
    }

    readFromDb(tx, false);

    return singleTask;
}

ResultData &Task::runCode(bool isDaily, const string &langExt) {
    string fileName;
    if (isDaily)
        fileName = "dailyTask.";
    else
        fileName = "singleTask.";
    std::ifstream myFileInput(fileName + langExt);
    std::stringstream ss;
    string typedCode;

    if (myFileInput.is_open()) {
        ss << myFileInput.rdbuf();
        typedCode = ss.str();
        myFileInput.close();
    }

    json response = RunCodeRequests::getResult(singleTask.id, singleTask.titleSlug, typedCode);

    resultData.statusMessage = response["status_msg"];
    if (resultData.statusMessage == "Accepted") {
        resultData.totalTestCases = response["total_testcases"];
        resultData.totalCorrect = response["total_correct"];
        resultData.statusMemory = response["status_memory"];
        resultData.statusRuntime = response["status_runtime"];
        resultData.codeAnswer = response["code_answer"];
        resultData.expectedCodeAnswer = response["expected_code_answer"];
    } else {
        resultData.statusMessage = response["status_msg"];
        resultData.fullCompileError = response["full_compile_error"];
    }

    return resultData;
}

string Task::submitCode() {
    return "hello";
}
