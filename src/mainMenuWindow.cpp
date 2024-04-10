#include "mainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(WINDOW* parentWin, const vector<string> &menuItems)
    : MenuWindow(parentWin, menuItems) {}

WINDOW* MainMenuWindow::drawWindow(int row, int col, int x, int y) {
    if (curWin != nullptr) return curWin;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);

    curWin = newwin(row, col, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Menu ");
    wattroff(curWin, COLOR_PAIR(3));

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
        }
    }

    return curWin;
}

void MainMenuWindow::refreshWindow(int row, int col, int x, int y) {
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
        if (i == curItem) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 2, 5, "%s", menuItems[i].c_str());
        }
    }
}

int MainMenuWindow::handleKeyEvent(Task* task) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp();
                if (curItem == 1) {
                    int rows, cols;
                    getmaxyx(parentWin, rows, cols);
                    mvwprintw(parentWin, rows - 2, 3, "Press 'r' to read the task and 'o' to open it in nvim");
                    continue;
                }
                return static_cast<int>(menuCodes::refreshWin);
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown();
                if (curItem == 1) {
                    int rows, cols;
                    getmaxyx(parentWin, rows, cols);
                    mvwprintw(parentWin, rows - 2, 3, "Press 'r' to read the task and 'o' to open it in nvim");
                    continue;
                }
                return static_cast<int>(menuCodes::refreshWin);
            }

            case 'r' : {
                if (curItem == 1) {
                    TextWindow dailyTaskTextWindow(task->getDailyTask().content);
                    int rows, cols;
                    getmaxyx(stdscr, rows, cols);
                    WINDOW* dailyTaskTextWin = dailyTaskTextWindow.drawWindow(30, 80, rows / 2 - 15, cols / 2 - 40);
                    wrefresh(dailyTaskTextWin);

                    dailyTaskTextWindow.handleKeyEvent();

                    return static_cast<int>(menuCodes::refreshWin);
                }
            }
            break;

            case 'o' : {
                if (curItem == 1) {
                }
            }
            break;

            case 'q' : return static_cast<int>(menuCodes::quit);
        }

    }

    return 0;
}