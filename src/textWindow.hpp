#pragma once

#include <ncurses.h>
#include <string>

using std::string;

class TextWindow {
    friend int main(int, char**);
    WINDOW* curWin;
    int rows;
    int cols;
    const string content;
    int contentLength;
    int contentLines;
    int startLine;
    void scrollUp();
    void scrollDown();
    void printWindowContent();
    void clearWindowContent();
public:
    TextWindow(const TextWindow&) = delete;
    TextWindow() = delete;
    TextWindow& operator=(const TextWindow&) = delete;
    explicit TextWindow(string content);
    WINDOW* drawWindow(int rows, int cols, int x, int y);
    void refreshWindow(int rows, int cols, int x, int y);
    void handleKeyEvent();
};