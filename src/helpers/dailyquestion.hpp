#pragma once

#include "dailyrequest.hpp"
#include <ctime>

class DailyQuestion {
    string question;
    int curDay = 0;
public:
    string getDailyQuestion() {
        std::time_t currentTime = std::time(nullptr);
        std::tm* localTime = std::localtime(&currentTime);
        int day = localTime->tm_mday;
        if (!question.empty() && day == curDay) return question;
        curDay = day;
        question = DailyRequest::getResponse();
        return question;
    }
};