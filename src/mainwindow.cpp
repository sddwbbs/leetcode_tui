#include "mainwindow.hpp"

WINDOW* MainWindow::curWin = nullptr;

WINDOW* MainWindow::drawWindow(int row, int col, int x, int y) {
    if (curWin != nullptr) return curWin;
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    curWin = newwin(row, col, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(2));
    box(curWin, x, y);
    wattroff(curWin, COLOR_PAIR(2));

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 0, col / 2 - 7, " LeetCode TUI ");
    wattroff(curWin, COLOR_PAIR(1));

    return curWin;
}

void MainWindow::refreshWindow(int row, int col, int x, int y) {
    wresize(curWin, row, col);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(2));
    box(curWin, x, y);
    wattroff(curWin, COLOR_PAIR(2));

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 0, col / 2 - 7, " LeetCode TUI ");
    wattroff(curWin, COLOR_PAIR(1));
}