#pragma once

#include <ncurses.h>
#include <string>

using std::string;

class TextWindow {
    friend int main(int, char**);
    WINDOW* curWin;
    const string content;
    const int contentLength;
    int totalLines;
public:
    TextWindow(const TextWindow&) = delete;
    TextWindow() = delete;
    TextWindow& operator=(const TextWindow&) = delete;
    explicit TextWindow(string content);
    WINDOW* drawWindow(int row, int col, int x, int y);
    void refreshWindow(int row, int col, int x, int y);
};