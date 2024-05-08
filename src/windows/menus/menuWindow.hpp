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
    itemSelected,
};

class MenuWindow {
protected:
    WINDOW *curWin = nullptr;
    WINDOW *parentWin = nullptr;
    vector<string> menuItems;
    int curItem = 0;
    int menuSize = 0;
    int rows = 0;
    int cols = 0;
    int x = 0;
    int y = 0;
    int rowsPadding = 0;
    int colsPadding = 0;

    void menuUp(int _rowsPadding, int _colsPadding);

    void menuDown(int _rowsPadding, int _colsPadding);

public:
    MenuWindow(const MenuWindow &) = delete;

    MenuWindow() = default;

    MenuWindow &operator=(const MenuWindow &) = delete;

    explicit MenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    WINDOW *drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding);

    void refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding);

    [[nodiscard]] const char *getMenuItem(int index) const;

    [[nodiscard]] int getCurItem() const;

    [[nodiscard]] int getMenuSize() const;

    virtual int handleKeyEvent();
};