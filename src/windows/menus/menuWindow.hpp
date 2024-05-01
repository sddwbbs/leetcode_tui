#pragma once

#include <ncurses.h>
#include <vector>
#include <pqxx/pqxx>
#include <string>

#include "../../helpers/task.hpp"

using std::vector;
using std::string;

enum class menuCodes : int {
    ok,
    quit,
    refreshWin,
    itemSelected
};

class MenuWindow {
protected:
    WINDOW *curWin = nullptr;
    WINDOW *parentWin = nullptr;
    const vector<string> menuItems;
    int curItem = 0;
    int menuSize = 0;

    void menuUp(int rowsPadding, int colsPadding);

    void menuDown(int rowsPadding, int colsPadding);

public:
    MenuWindow(const MenuWindow &) = delete;

    MenuWindow() = default;

    MenuWindow &operator=(const MenuWindow &) = delete;

    explicit MenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    WINDOW *drawWindow(int row, int col, int x, int y, int rowsPadding, int colsPadding);

    void refreshWindow(int row, int col, int x, int y, int rowsPadding, int colsPadding);

    [[nodiscard]] const char *getMenuItem(int index) const;

    [[nodiscard]] int getCurItem() const;

    [[nodiscard]] int getMenuSize() const;

    virtual int handleKeyEvent(Task *task) = 0;
};