#include "launchMenuWindow.hpp"

LaunchMenuWindow::LaunchMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {}

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