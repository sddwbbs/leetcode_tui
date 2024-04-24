#include "launchMenuWindow.hpp"

LaunchMenuWindow::LaunchMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {}

WINDOW * LaunchMenuWindow::drawWindow(int row, int col, int x, int y) {
    if (curWin != nullptr) return curWin;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);

    curWin = newwin(row, col, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + 1, 1, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 1, 1, "%s", menuItems[i].c_str());
        }
    }

    return curWin;
}

void LaunchMenuWindow::refreshWindow(int row, int col, int x, int y) {
    wresize(curWin, row, col);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(4));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + 1, 1, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 1, 1, "%s", menuItems[i].c_str());
        }
    }
}

int LaunchMenuWindow::handleKeyEvent(Task *task) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(1, 1);
                return static_cast<int>(menuCodes::ok);
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(1, 1);
                return static_cast<int>(menuCodes::ok);
            }

            case 10 : {
                if (curItem == 0) {
                    string title = " Result ";
                    string content = task->runCode();
                    TextWindow resultTextWindow(title, content);
                    int rows, cols;
                    getmaxyx(stdscr, rows, cols);
                    WINDOW *dailyTaskTextWin = resultTextWindow.drawWindow(16, 40, rows / 2 - 8, cols / 2 - 20);
                    wrefresh(dailyTaskTextWin);

                    resultTextWindow.handleKeyEvent();

                    return static_cast<int>(menuCodes::quit);
                }
            }

            case 'q' : {
                return static_cast<int>(menuCodes::quit);
            }

            default:
                return static_cast<int>(menuCodes::ok);
        }
    }

    return static_cast<int>(menuCodes::ok);
}