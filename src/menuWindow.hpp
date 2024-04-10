#pragma once

#include <ncurses.h>
#include <vector>
#include <string>

#include "helpers/task.hpp"

using std::vector;
using std::string;

enum class menuCodes : int {
    ok,
    quit,
    refreshWin
};

class MenuWindow {
protected:
    WINDOW* curWin = nullptr;
    WINDOW* parentWin = nullptr;
    const vector<string> menuItems;
    int curItem = 0;
    int menuSize = 0;
    void menuUp();
    void menuDown();
public:
    MenuWindow(const MenuWindow&) = delete;
    MenuWindow() = delete;
    MenuWindow& operator=(const MenuWindow&) = delete;
    explicit MenuWindow(WINDOW* parentWin, const vector<string> &menuItems);
    virtual WINDOW* drawWindow(int row, int col, int x, int y) = 0;
    virtual void refreshWindow(int row, int col, int x, int y) = 0;
    const char* getMenuItem(int index);
    int getCurItem() const;
    int getMenuSize() const;
    virtual int handleKeyEvent(Task* task) = 0;
};