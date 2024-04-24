#include "menuWindow.hpp"

MenuWindow::MenuWindow(WINDOW* parentWin, const vector<string> &menuItems)
    : parentWin(parentWin)
    , menuItems(menuItems)
    , menuSize(menuItems.size()) {}

const char* MenuWindow::getMenuItem(int index) {
    return menuItems[index].c_str();
}

int MenuWindow::getCurItem() const { return curItem; }

int MenuWindow::getMenuSize() const { return menuSize; }

void MenuWindow::menuUp(int rowsPadding, int colsPadding) { // 2 5
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    --curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wrefresh(curWin);
}

void MenuWindow::menuDown(int rowsPadding, int colsPadding) { // 2 5
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    ++curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + rowsPadding, colsPadding, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wrefresh(curWin);
}
