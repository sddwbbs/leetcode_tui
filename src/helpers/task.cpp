#include "task.hpp"

Task::Task()
    : dailyRefreshTime(0)
    , dailyTask("", "", "", "")
    , singleTask("", "", "", "") {}


TaskData& Task::getDailyTask() {
    std::time_t curTime = std::time(nullptr);

    dailyRefreshTime = curTime;
    std::tm* refreshDate = std::localtime(&dailyRefreshTime);
    refreshDate->tm_hour = 7;
    refreshDate->tm_min = 0;
    dailyRefreshTime = std::mktime(refreshDate);

    string firstLine;
    bool fileOpened = false;
    time_t fileRefreshTime = dailyRefreshTime;
    std::ifstream myFileInput("content.txt");
    if (myFileInput.is_open()) {
        fileOpened = true;
        std::getline(myFileInput >> std::ws, firstLine);
        std::stringstream buffer;
        std::getline(myFileInput >> std::ws, dailyTask.title);
        buffer << dailyTask.title << myFileInput.rdbuf();
        dailyTask.content = buffer.str();
        fileRefreshTime = std::stoi(firstLine);
        if (fileRefreshTime > dailyRefreshTime) dailyRefreshTime = fileRefreshTime;
        myFileInput.close();
    }

    if (!fileOpened || curTime >= dailyRefreshTime || curTime >= fileRefreshTime) {
        refreshDate->tm_mday += 1;
        dailyRefreshTime = std::mktime(refreshDate);
        json jsonResponse;

        jsonResponse = DailyTaskRequest::getTitleSlug();
        dailyTask.titleSlug = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["titleSlug"];
        dailyTask.title = jsonResponse["data"]["activeDailyCodingChallengeQuestion"]["question"]["title"];

        jsonResponse = DailyTaskRequest::getContent(dailyTask.titleSlug);
        dailyTask.htmlContent = jsonResponse["data"]["question"]["content"];
        if (DailyTaskRequest::saveToFile(dailyTask, dailyRefreshTime))
            return dailyTask;
    }

    return dailyTask;
}

TaskData& Task::getSingleTask() {
    json jsonResponse;

    jsonResponse = DailyTaskRequest::getContent(singleTask.titleSlug);
    dailyTask.htmlContent = jsonResponse["data"]["question"]["content"];

    return singleTask;
}

string Task::getCodeSnippet(string &titleSlug, string language, TaskData& task) {
    json jsonResponse;

    jsonResponse = CodeSnippetRequest::getCodeSnippet(titleSlug, language);

    for (const auto& snippet : jsonResponse["data"]["question"]["codeSnippets"]) {
        string lang = snippet["lang"];
        if (lang == language) {
            task.htmlCodeSnippet = snippet["code"];
            std::ofstream myFileOutput;
            myFileOutput.open("codeSnippet.html");
            myFileOutput << task.htmlCodeSnippet;
            myFileOutput.close();

            system("elinks -dump codeSnippet.html > codeSnippet.cpp");

            std::ifstream myFileInput ("codeSnippet.cpp");
            if (myFileInput.is_open()) {
                std::stringstream buffer;
                buffer << myFileInput.rdbuf();
                task.codeSnippet = buffer.str();
            } else return "Problems with saving file";
            myFileInput.close();

            return task.codeSnippet;
        }
    }

    return "There is no such language :(";
}

string Task::runCode() {
    return "hello";
}

string Task::submitCode() {
    return "hello";
}
