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
    int x;
    int y;
    const string &title;
    const string &content;
    int contentLength;
    int contentLines;
    int startLine;

    void scrollUp();

    void scrollDown();

    void printWindowContent() const;

public:
    TextWindow(const TextWindow &) = delete;

    TextWindow() = delete;

    TextWindow &operator=(const TextWindow &) = delete;

    explicit TextWindow(const string &title, const string &content);

    WINDOW *drawWindow(int rows, int cols, int x, int y, int boxColorPair);

    void refreshWindow(int rows, int cols, int x, int y, int boxColorPair);

    void handleKeyEvent();

    void clearWindowContent() const;
};