#pragma once

#include <ncurses.h>

class MainWindow {
    static WINDOW* curWin;
    static int rows;
    static int cols;
    static int x;
    static int y;
public:
    MainWindow() = default;
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

    static WINDOW* drawWindow(int row, int col, int x, int y);
    static void refreshWindow(int row, int col, int x, int y);
    static void clearWindowContent();
};