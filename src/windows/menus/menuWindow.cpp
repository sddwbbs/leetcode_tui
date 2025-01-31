#include "menuWindow.hpp"

MenuWindow::MenuWindow(WINDOW *parentWin)
    : parentWin(parentWin) {
}

MenuWindow::MenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
    : parentWin(parentWin)
      , menuItems(menuItems) {
}

MenuWindow::MenuWindow(WINDOW *parentWin, const vector<string> &menuItems, string menuTitle)
    : parentWin(parentWin)
      , menuItems(menuItems)
      , menuTitle(std::move(menuTitle)) {
}

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

    for (int curElemIdx = 0; curElemIdx < menuItems.size(); ++curElemIdx) {
        if (curElemIdx == selectedItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
        }
    }

    return curWin;
}

void MenuWindow::refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) {
    bool somethingChanged = false;
    if (_rows != rows || _cols != cols || _x != x || _y != y || _rowsPadding != rowsPadding || _colsPadding !=
        colsPadding)
        somethingChanged = true;

    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;

    if (somethingChanged) {
        wresize(curWin, rows, cols);
        mvwin(curWin, x, y);
    }

    werase(curWin);

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(0));

    if (!menuTitle.empty()) {
        wattron(curWin, COLOR_PAIR(0));
        mvwprintw(curWin, 0, 3, " %s ", menuTitle.c_str());
        wattroff(curWin, COLOR_PAIR(0));
    }

    for (int curElemIdx = 0; curElemIdx < menuItems.size(); ++curElemIdx) {
        if (curElemIdx == selectedItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
        }
    }
}

void MenuWindow::menuUp(const int _rowsPadding, const int _colsPadding) {
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;

    if (selectedItemIdx == 0) return;

    mvwprintw(curWin, selectedItemIdx + rowsPadding, colsPadding, "%s",
              menuItems[selectedItemIdx].c_str());
    --selectedItemIdx;

    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, selectedItemIdx + rowsPadding, colsPadding, "%s",
              menuItems[selectedItemIdx].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
    wnoutrefresh(curWin);
}

void MenuWindow::menuDown(const int _rowsPadding, const int _colsPadding) {
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;

    if (selectedItemIdx == menuItems.size() - 1) return;

    mvwprintw(curWin, selectedItemIdx + rowsPadding, colsPadding, "%s",
              menuItems[selectedItemIdx].c_str());
    ++selectedItemIdx;

    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, selectedItemIdx + rowsPadding, colsPadding, "%s",
              menuItems[selectedItemIdx].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
    wnoutrefresh(curWin);
}

void MenuWindow::scrollUp() {
    if (pageNumber == 0) return;

    if (!pageLastIdxs.empty()) {
        selectedItemIdx = pageLastIdxs.top();
        pageLastIdxs.pop();
    }
    if (!beginIdxs.empty()) {
        beginOfDisplayedItemsIdx = beginIdxs.top();
        beginIdxs.pop();
    }
    if (pageNumber - 1 >= 0) --pageNumber;
    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
    wnoutrefresh(curWin);
}

void MenuWindow::scrollDown() {
    if (pageNumber == pageLimit - 1) return;

    pageLastIdxs.push(selectedItemIdx);
    beginIdxs.push(beginOfDisplayedItemsIdx);
    beginOfDisplayedItemsIdx += static_cast<int>(menuItems.size());
    selectedItemIdx = 0;
    ++pageNumber;
    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
    wnoutrefresh(curWin);
}

const char *MenuWindow::getMenuItem(const int index) const { return menuItems[index].c_str(); }

string MenuWindow::getCurItem() const { return menuItems[selectedItemIdx]; }

int MenuWindow::getMenuSize() const { return static_cast<int>(menuItems.size()); }

int MenuWindow::getCurItemIdx() const { return selectedItemIdx; };

menuCodes MenuWindow::handleKeyEvent() {
    return menuCodes::ok;
}