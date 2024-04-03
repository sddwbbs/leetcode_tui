#include "menuwindow.hpp"

MenuWindow::MenuWindow(const vector<string> &menuItems) : menuItems(menuItems), menuSize(menuItems.size()) {}

WINDOW* MenuWindow::drawWindow(int row, int col, int x, int y) {
    if (curWin != nullptr) return curWin;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // Пара цветов для желтого текста на голубом фоне

    curWin = newwin(row, col, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(4)); // Применяем цвет фона
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Menu ");
    wattroff(curWin, COLOR_PAIR(3));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) { // Если текущий элемент, то применяем желтый текст на голубом фоне
            wattron(curWin, COLOR_PAIR(5)); // Применяем пару цветов для желтого текста на голубом фоне
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
        }
    }

    return curWin;
}

void MenuWindow::refreshWindow(int row, int col, int x, int y) {
    wresize(curWin, row, col);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Menu ");
    wattroff(curWin, COLOR_PAIR(3));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) { // Если текущий элемент, то применяем желтый текст на голубом фоне
            wattron(curWin, COLOR_PAIR(5)); // Применяем пару цветов для желтого текста на голубом фоне
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
        }
    }
}

const char* MenuWindow::getMenuItem(int index) {
    return menuItems[index].c_str();
}

void MenuWindow::menuUp() {
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    --curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));
}

void MenuWindow::menuDown() {
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    ++curItem;
    wattron(curWin, COLOR_PAIR(5));
    mvwprintw(curWin, curItem + 2, 5, "%s", menuItems[curItem].c_str());
    wattroff(curWin, COLOR_PAIR(5));
}