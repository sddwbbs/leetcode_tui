#include "mainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {}

WINDOW *MainMenuWindow::drawWindow(int row, int col, int x, int y) {
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

int MainMenuWindow::handleKeyEvent(Task *task) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(2, 5);
                return static_cast<int>(menuCodes::refreshWin);
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(2, 5);
                return static_cast<int>(menuCodes::refreshWin);
            }

            case 'r' : {
                if (curItem == 1) {
                    TextWindow dailyTaskTextWindow(task->getDailyTask().title, task->getDailyTask().content);
                    int rows, cols;
                    getmaxyx(stdscr, rows, cols);
                    WINDOW *dailyTaskTextWin = dailyTaskTextWindow.drawWindow(30, 80, rows / 2 - 15, cols / 2 - 40);
                    wrefresh(dailyTaskTextWin);

                    dailyTaskTextWindow.handleKeyEvent();

                    return static_cast<int>(menuCodes::refreshWin);
                }
            }
                break;

            case 'o' : {
                if (curItem == 1) {
                    if (refreshCodeSnippetStatus) {
                        std::ofstream myFileInput;

                        string codeSnippet;
                        for (const auto &item: task->getDailyTask().codeSnippets) {
                            if (item["langSlug"] == "cpp") {
                                codeSnippet = item["code"];
                                break;
                            }
                        }

                        myFileInput.open("dailyTask.cpp");
                        myFileInput << codeSnippet;
                        myFileInput.close();
                        refreshCodeSnippetStatus = false;
                    }
                    system("nvim dailyTask.cpp");
                    endwin();
                    initscr();
                    return static_cast<int>(menuCodes::refreshWin);
                }
            }
                break;

            case 'c' : {
                if (curItem == 1) {
                    refreshCodeSnippetStatus = true;
                    return static_cast<int>(menuCodes::refreshWin);
                }
            }
                break;

            case 10 : {
                if (curItem == 0) {
                    system("nvim");
                    endwin();
                    initscr();
                    return static_cast<int>(menuCodes::refreshWin);
                }

                if (curItem == 1 && !refreshCodeSnippetStatus) {
                    LaunchMenuWindow launchMenuWindow(curWin, {"  Run     ", "  Submit  "});
                    int rows, cols;
                    getmaxyx(stdscr, rows, cols);
                    WINDOW *launchMenuWin = launchMenuWindow.drawWindow(4, 12, rows / 2, cols / 2 + 18);
                    wrefresh(launchMenuWin);

                    while (true) {
                        int curCode = launchMenuWindow.handleKeyEvent(task);
                        if (curCode == static_cast<int>(menuCodes::quit)) break;
                        if (curCode == static_cast<int>(menuCodes::ok))
                            wrefresh(launchMenuWin);
                    }

                    return static_cast<int>(menuCodes::refreshWin);
                }
            }
                break;

            case 'q' :
                return static_cast<int>(menuCodes::quit);

            default:
                return static_cast<int>(menuCodes::ok);
        }
    }

    return static_cast<int>(menuCodes::ok);
}

bool MainMenuWindow::getRefreshCodeSnippetStatus() const {
    return refreshCodeSnippetStatus;
}