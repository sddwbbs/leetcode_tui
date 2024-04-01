#pragma once

#include <ncurses.h>

class MainWindow {
    static WINDOW* curWin;
    MainWindow() = default;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
public:
    static WINDOW* drawWindow(int row, int col, int x, int y);
    static void refreshWindow(int row, int col, int x, int y);
};