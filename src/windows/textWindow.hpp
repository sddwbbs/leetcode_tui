#pragma once

#include <ncurses.h>
#include <string>

#include "../helpers/task.hpp"

using std::string;

struct TaskData;

class TextWindow {
    WINDOW *curWin;
    int rows;
    int cols;
    TaskData &taskData;
    int contentLength;
    int contentLines;
    int startLine;
    int startSymbol;

    void moveStartSymbol();

    void scrollUp();

    void scrollDown();

    void printWindowContent();

    void clearWindowContent();

public:
    TextWindow(const TextWindow &) = delete;

    TextWindow() = delete;

    TextWindow &operator=(const TextWindow &) = delete;

    explicit TextWindow(TaskData &taskData);

    WINDOW *drawWindow(int rows, int cols, int x, int y);

    void refreshWindow(int rows, int cols, int x, int y);

    void handleKeyEvent();
};