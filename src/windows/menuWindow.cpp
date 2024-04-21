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

void MenuWindow::menuUp() {
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    --curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wrefresh(curWin);
}

void MenuWindow::menuDown() {
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    ++curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));

    wrefresh(curWin);
}
