#include "task.hpp"

Task::Task(pqxx::connection &conn)
        : conn(conn) {}

void Task::extractText(GumboNode *node, std::string &plainText) {
    if (node->type == GUMBO_NODE_TEXT) {
        plainText.append(node->v.text.text);
    } else if (node->type == GUMBO_NODE_ELEMENT &&
               node->v.element.tag != GUMBO_TAG_SCRIPT &&
               node->v.element.tag != GUMBO_TAG_STYLE) {
        if (node->v.element.tag == GUMBO_TAG_SUP) {
            plainText.append("^");
        }
        plainText.append(" ");
        GumboVector *children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            extractText(static_cast<GumboNode *>(children->data[i]), plainText);
        }
    }
}

void Task::saveToDb(bool dailyInDb, pqxx::work &tx) {
    if (dailyInDb)
        try {
            tx.exec("UPDATE leetcode_tui.tasks SET refresh_time = NULL WHERE refresh_time IS NOT NULL");
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
                "SELECT COUNT(*) FROM leetcode_tui.tasks WHERE id = $1", singleTask.id);
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }

    if (alreadyInTable) {
        try {
            tx.exec_params("UPDATE leetcode_tui.tasks SET refresh_time = $1 WHERE id = $2", refreshedTime,
                           singleTask.id);
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << '\n';
        }
    } else {
        try {
            tx.exec_params(
                    "INSERT INTO leetcode_tui.tasks (id, frontend_id, title_slug, title, difficulty, content, topic_tags, code_snippets, paid_only, refresh_time) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10)",
                    singleTask.id, singleTask.frontendId, singleTask.titleSlug, singleTask.title,
                    singleTask.difficulty, singleTask.content, singleTask.topicTags.dump(),
                    singleTask.codeSnippets.dump(), singleTask.paidOnly, refreshedTime);
        } catch (std::exception const &e) {
            std::cerr << "ERROR: " << e.what() << '\n';
        }
    }
}

void Task::readFromDb(pqxx::work &tx) {
    try {
        pqxx::result result = tx.exec(
                "SELECT id, frontend_id, title_slug, title, difficulty, content, topic_tags, code_snippets, paid_only "
                "FROM leetcode_tui.tasks WHERE refresh_time IS NOT NULL");

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
}

TaskData &Task::getDailyTask() {
    std::time_t curTime = std::time(nullptr);
    std::tm *timeInfo = std::localtime(&curTime);
    std::tm *refreshDate = timeInfo;

    pqxx::work tx{conn};

    bool dailyInDb = false;
    try {
        dailyInDb = tx.query_value<bool>(
                "SELECT COUNT(*) FROM leetcode_tui.tasks WHERE refresh_time IS NOT NULL");
    } catch (std::exception const &e) {
        std::cerr << '\n' << "ERROR: " << e.what() << '\n';
    }

    time_t refreshTimeFromDb = 0;
    if (dailyInDb) {
        try {
            refreshTimeFromDb = tx.query_value<time_t>(
                    "SELECT refresh_time FROM leetcode_tui.tasks WHERE refresh_time IS NOT NULL");
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

        GumboOutput *output = gumbo_parse(singleTask.content.c_str());
        if (output) {
            singleTask.content = "";
            extractText(output->root, singleTask.content);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
        } else {
            std::cerr << "Error parsing HTML.\n";
        }

        singleTask.topicTags = jsonDailyTask["topicTags"];
        singleTask.codeSnippets = jsonDailyTask["codeSnippets"];
        singleTask.paidOnly = jsonDailyTask["paidOnly"].get<bool>();

        saveToDb(dailyInDb, tx);

        tx.commit();

        return singleTask;
    }

    readFromDb(tx);

    return singleTask;
}

TaskData &Task::getSingleTask() {
    return singleTask;
}

string Task::runCode() {
    return "hello";
}

string Task::submitCode() {
    return "hello";
}
