#pragma once

#include <ncurses.h>
#include <vector>
#include <pqxx/pqxx>
#include <string>
#include <utility>
#include <stack>

#include "../../helpers/task.hpp"

enum class menuCodes : int {
    ok,
    quit,
    refreshWin,
    itemSelected,
};

using std::vector;
using std::string;
using std::stack;

class MenuWindow {
protected:
    WINDOW *curWin = nullptr;
    WINDOW *parentWin = nullptr;
    int selectedItemIdx = 0;
    int pageNumber = 0;
    int pageLimit = 0;
    stack<int> pageLastIdxs;
    stack<int> beginIdxs;
    vector<string> menuItems;
    string menuTitle;
    int beginOfDisplayedItemsIdx = 0;
    int endOfDisplayedItemsIdx = 0;
    int rows = 0;
    int cols = 0;
    int x = 0;
    int y = 0;
    int rowsPadding = 0;
    int colsPadding = 0;

    void menuUp(int _rowsPadding, int _colsPadding);

    void menuDown(int _rowsPadding, int _colsPadding);

    void scrollUp();

    void scrollDown();

public:
    MenuWindow(const MenuWindow &) = delete;

    MenuWindow() = default;

    MenuWindow &operator=(const MenuWindow &) = delete;

    virtual ~MenuWindow() = default;

    explicit MenuWindow(WINDOW *parentWin);

    explicit MenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    explicit MenuWindow(WINDOW *parentWin, const vector<string> &menuItems, string menuTitle);

    virtual WINDOW *drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding);

    virtual void refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding);

    [[nodiscard]] const char *getMenuItem(int index) const;

    [[nodiscard]] string getCurItem() const;

    [[nodiscard]] int getMenuSize() const;

    [[nodiscard]] int getCurItemIdx() const;

    virtual menuCodes handleKeyEvent();
};
