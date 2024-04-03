#pragma once

#include <ncurses.h>
#include <vector>
#include <string>

using std::vector;
using std::string;

class MenuWindow {
    WINDOW* curWin = nullptr;
    const vector<string> menuItems;
public:
    MenuWindow(const MenuWindow&) = delete;
    MenuWindow() = delete;
    MenuWindow& operator=(const MenuWindow&) = delete;
    explicit MenuWindow(const vector<string> &menuItems);
    int curItem = 0;
    int menuSize = 0;
    WINDOW* drawWindow(int row, int col, int x, int y);
    void refreshWindow(int row, int col, int x, int y);
    const char* getMenuItem(int index);
    void menuUp();
    void menuDown();
};