#include "mainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {
    getmaxyx(parentWin, rows, cols);
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
                    WINDOW *dailyTaskTextWin = dailyTaskTextWindow.drawWindow(rows / 2 + rows / 4, cols / 2, rows / 6,
                                                                              cols / 2 - (cols / 4));
                    wrefresh(dailyTaskTextWin);

                    dailyTaskTextWindow.handleKeyEvent();

                    return static_cast<int>(menuCodes::refreshWin);
                }
            }
                break;

            case 'o' : {
                if (curItem == 1) {
                    if (refreshCodeSnippetStatus) {
                        std::ofstream myFileOutput;
                        string selectedLang;
                        string codeSnippet;
                        int selectedLangIndex = -1;

                        LanguageMenuWindow languageMenuWindow(curWin);
                        WINDOW *languageMenuWin = languageMenuWindow.drawWindow(23, 40, rows / 2 - 11, cols / 2 - 20, 2,
                                                                                5);
                        wrefresh(languageMenuWin);

                        while (true) {
                            int curCode = languageMenuWindow.handleKeyEvent();
                            if (curCode == static_cast<int>(menuCodes::itemSelected)) {
                                selectedLangIndex = languageMenuWindow.getCurItem();
                                break;
                            }
                            if (curCode == static_cast<int>(menuCodes::quit)) break;
                            if (curCode == static_cast<int>(menuCodes::ok))
                                wrefresh(languageMenuWin);
                        }

                        if (selectedLangIndex == -1)
                            return static_cast<int>(menuCodes::refreshWin);
                        selectedLang = lang[selectedLangIndex];
                        auto pos = languageMenuWindow.langExtMap.find(selectedLang);
                        if (pos != languageMenuWindow.langExtMap.end())
                            langExt = pos->second;

                        for (const auto &item: task->getDailyTask().codeSnippets) {
                            if (item["lang"] == selectedLang) {
                                codeSnippet = item["code"];
                                break;
                            }
                        }

                        myFileOutput.open("dailyTask." + langExt);
                        myFileOutput << codeSnippet;
                        myFileOutput.close();
                        refreshCodeSnippetStatus = false;
                    }

                    string command = "nvim dailyTask." + langExt;
                    system(command.c_str());
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

                if (curItem == 3) {
                    MainWindow::clearWindowContent();
                    wrefresh(parentWin);

                    SearchBarWindow searchBarWindow;
                    WINDOW *searchBarWin = searchBarWindow.drawWindow(3, cols / 2 + cols / 4, 2, cols / 8);
                    wrefresh(searchBarWin);

                    SearchResultsMenu searchResultsMenu;
                    WINDOW *searchResultMenuWin = searchResultsMenu.drawWindow(rows / 2 + rows / 4, cols / 2 + cols / 4, 5, cols / 8, 4, 4);
                    wrefresh(searchResultMenuWin);

                    curs_set(1);
                    wmove(stdscr, 3, cols / 8 + 2);
                    searchBarWindow.handleKeyEvent();
                    curs_set(0);

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