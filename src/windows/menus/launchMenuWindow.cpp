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
                    ResultData resultData = task->runCode();
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
                        dailyTaskTextWin = resultTextWindow.drawWindow(16, 40, rows / 2 - 8, cols / 2 - 20);
                    else
                        dailyTaskTextWin = resultTextWindow.drawWindow(16, 80, rows / 2 - 8, cols / 2 - 40);
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