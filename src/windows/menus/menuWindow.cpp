#include "menuWindow.hpp"

#include <utility>

MenuWindow::MenuWindow(WINDOW *parentWin)
    : parentWin(parentWin) {}

MenuWindow::MenuWindow(WINDOW* parentWin, const vector<string> &menuItems)
    : parentWin(parentWin)
    , menuItems(menuItems)
    , menuSize(static_cast<int>(menuItems.size())) {}

MenuWindow::MenuWindow(WINDOW* parentWin, const vector<string> &menuItems, string menuTitle)
    : parentWin(parentWin)
    , menuItems(menuItems)
    , menuTitle(std::move(menuTitle))
    , menuSize(static_cast<int>(menuItems.size())) {}

WINDOW *MenuWindow::drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;

    curWin = newwin(rows, cols, y, x);
    refresh();

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(0));

    if (!menuTitle.empty()) {
        wattron(curWin, COLOR_PAIR(0));
        mvwprintw(curWin, 0, 3, " %s ", menuTitle.c_str());
        wattroff(curWin, COLOR_PAIR(0));
    }

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
        }
    }

    return curWin;
}

void MenuWindow::refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) {
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    wresize(curWin, rows, cols);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(0));

    if (!menuTitle.empty()) {
        wattron(curWin, COLOR_PAIR(0));
        mvwprintw(curWin, 0, 3, " %s ", menuTitle.c_str());
        wattroff(curWin, COLOR_PAIR(0));
    }

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
        }
    }
}

const char* MenuWindow::getMenuItem(int index) const {
    return menuItems[index].c_str();
}

int MenuWindow::getCurItemIdx() const { return curItemIdx; }

string MenuWindow::getCurItem() const { return menuItems[curItemIdx]; }

int MenuWindow::getMenuSize() const { return menuSize; }

void MenuWindow::menuUp(int _rowsPadding, int _colsPadding) {
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    mvwprintw(curWin, curItemIdx + rowsPadding, colsPadding, "%s", menuItems[curItemIdx].c_str());
    --curItemIdx;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItemIdx + rowsPadding, colsPadding, "%s", menuItems[curItemIdx].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wnoutrefresh(curWin);
}

void MenuWindow::menuDown(int _rowsPadding, int _colsPadding) {
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    mvwprintw(curWin, curItemIdx + rowsPadding, colsPadding, "%s", menuItems[curItemIdx].c_str());
    ++curItemIdx;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItemIdx + rowsPadding, colsPadding, "%s", menuItems[curItemIdx].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wnoutrefresh(curWin);
}

menuCodes MenuWindow::handleKeyEvent() {
    return menuCodes::ok;
}