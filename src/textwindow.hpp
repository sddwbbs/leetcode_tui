#pragma once

#include <ncurses.h>
#include <string>

using std::string;

class TextWindow {
    WINDOW* curWin;
    const string content;
    int scrollPosition;
public:
    TextWindow(const TextWindow&) = delete;
    TextWindow() = delete;
    TextWindow& operator=(const TextWindow&) = delete;
    explicit TextWindow(string content);
    WINDOW* drawWindow(int row, int col, int x, int y);
    void refreshWindow(int row, int col, int x, int y);
    void handleKeyEvent(int key);
    void closeWindow();
    void refreshText();
    void scrollUp();
    void scrollDown();
};