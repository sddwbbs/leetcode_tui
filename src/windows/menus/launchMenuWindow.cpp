#include "launchMenuWindow.hpp"

LaunchMenuWindow::LaunchMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {}

menuCodes LaunchMenuWindow::handleKeyEvent(Task *task, bool isDaily, const string &langExt) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(1, 1);
                return menuCodes::refreshWin;
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(1, 1);
                return menuCodes::refreshWin;
            }

            case 10 : {
                if (curItem == 0) {
                    ResultData resultData = task->runCode(isDaily, langExt);
                    string title;
                    bool isCompileError = false;
                    string content;

                    if (resultData.statusMessage == "Accepted") {
                        if (resultData.totalTestCases != resultData.totalCorrect) {
                            title = " Wrong answer ";
                            content += "Output: \n";
                            for (auto &elem : resultData.codeAnswer) {
                                content += elem + '\n';
                            }
                            content += "\nExpected: \n";
                            for (auto &elem : resultData.expectedCodeAnswer) {
                                content += elem + '\n';
                            }
                        } else {
                            title = " Accepted ";
                            content = "Time: " + resultData.statusRuntime + '\n' + "Memory: " + resultData.statusMemory + '\n';
                        }
                    } else {
                        title = "Compile error";
                        isCompileError = true;
                        content = resultData.fullCompileError;
                    }

                    TextWindow resultTextWindow(title, content);
                    int rows, cols;
                    getmaxyx(stdscr, rows, cols);

                    WINDOW *dailyTaskTextWin = nullptr;
                    if (!isCompileError)
                        dailyTaskTextWin = resultTextWindow.drawWindow(16, 40, rows / 2 - 8, cols / 2 - 20, 3);
                    else
                        dailyTaskTextWin = resultTextWindow.drawWindow(16, 80, rows / 2 - 8, cols / 2 - 40, 3);
                    wrefresh(dailyTaskTextWin);

                    resultTextWindow.handleKeyEvent();

                    return menuCodes::quit;
                }
            }

            case 'q' : {
                return menuCodes::quit;
            }
        }
    }

    return menuCodes::ok;
}