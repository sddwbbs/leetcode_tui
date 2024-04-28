#include "mainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {}

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
                        string selectedLang;

                        LanguageMenuWindow languageMenuWindow(curWin);
                        int rows, cols;
                        getmaxyx(stdscr, rows, cols);
                        WINDOW *languageMenuWin = languageMenuWindow.drawWindow(23, 40, rows / 2 - 11, cols / 2 - 20, 2, 5);
                        wrefresh(languageMenuWin);

                        //bag when menu is closed without lang chosen
                        while (true) {
                            int curCode = languageMenuWindow.handleKeyEvent(task);
                            if (curCode == static_cast<int>(menuCodes::quit)) break;
                            if (curCode == static_cast<int>(menuCodes::ok))
                                wrefresh(languageMenuWin);
                        }

                        string codeSnippet;
                        for (const auto &item: task->getDailyTask().codeSnippets) {
                            if (item["langSlug"] == lang) {
                                codeSnippet = item["code"];
                                break;
                            }
                        }

                        myFileInput.open("dailyTask.cpp");
                        myFileInput << codeSnippet;
                        myFileInput.close();
                        refreshCodeSnippetStatus = false;

                        return static_cast<int>(menuCodes::refreshWin);
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
                    WINDOW *launchMenuWin = launchMenuWindow.drawWindow(4, 12, rows / 2, cols / 2 + 18, 1, 1);
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