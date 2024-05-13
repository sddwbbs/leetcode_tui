#include "menuWindow.hpp"

MenuWindow::MenuWindow(WINDOW* parentWin, const vector<string> &menuItems)
    : parentWin(parentWin)
    , menuItems(menuItems)
    , menuSize(static_cast<int>(menuItems.size())) {}

MenuWindow::MenuWindow(WINDOW *parentWin)
    : parentWin(parentWin) {}

WINDOW *MenuWindow::drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);

    curWin = newwin(rows, cols, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
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

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(4));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
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

int MenuWindow::getCurItem() const { return curItem; }

int MenuWindow::getMenuSize() const { return menuSize; }

void MenuWindow::menuUp(int _rowsPadding, int _colsPadding) {
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    --curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wnoutrefresh(curWin);
}

void MenuWindow::menuDown(int _rowsPadding, int _colsPadding) {
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    ++curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wnoutrefresh(curWin);
}

menuCodes MenuWindow::handleKeyEvent() {
    return menuCodes::ok;
}
