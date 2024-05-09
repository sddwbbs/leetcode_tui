#include "mainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {
    getmaxyx(parentWin, rows, cols);
}

menuCodes MainMenuWindow::handleKeyEvent(Task *task) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(2, 5);
                return menuCodes::refreshWin;
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(2, 5);
                return menuCodes::refreshWin;
            }

            case 'r' : {
                if (curItem == 1) {
                    TextWindow dailyTaskTextWindow(task->getDailyTask().title, task->getDailyTask().content);
                    WINDOW *dailyTaskTextWin = dailyTaskTextWindow.drawWindow(rows / 2 + rows / 4, cols / 2, rows / 6,
                                                                              cols / 2 - (cols / 4));
                    wrefresh(dailyTaskTextWin);

                    dailyTaskTextWindow.handleKeyEvent();

                    return menuCodes::refreshWin;
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
                            menuCodes curCode = languageMenuWindow.handleKeyEvent();
                            if (curCode == menuCodes::itemSelected) {
                                selectedLangIndex = languageMenuWindow.getCurItem();
                                break;
                            }
                            if (curCode == menuCodes::quit) break;
                            if (curCode == menuCodes::ok)
                                wrefresh(languageMenuWin);
                        }

                        if (selectedLangIndex == -1)
                            return menuCodes::refreshWin;
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
                    return menuCodes::refreshWin;
                }
            }
                break;

            case 'c' : {
                if (curItem == 1) {
                    refreshCodeSnippetStatus = true;
                    return menuCodes::refreshWin;
                }
            }
                break;

            case 10 : {
                if (curItem == 0) {
                    system("nvim");
                    endwin();
                    initscr();
                    return menuCodes::refreshWin;
                }

                if (curItem == 1 && !refreshCodeSnippetStatus) {
                    LaunchMenuWindow launchMenuWindow(curWin, {"  Run     ", "  Submit  "});
                    WINDOW *launchMenuWin = launchMenuWindow.drawWindow(4, 12, rows / 2, cols / 2 + 18, 1, 1);
                    wrefresh(launchMenuWin);

                    while (true) {
                        menuCodes curCode = launchMenuWindow.handleKeyEvent(task);
                        if (curCode == menuCodes::quit) break;
                        if (curCode == menuCodes::ok)
                            wrefresh(launchMenuWin);
                    }

                    return menuCodes::refreshWin;
                }

                if (curItem == 3) {
                    MainWindow::clearWindowContent();
                    wrefresh(parentWin);

                    SearchBarWindow searchBarWindow;
                    WINDOW *searchBarWin = searchBarWindow.drawWindow(3, cols / 2 + cols / 4, 2, cols / 8);
                    wrefresh(searchBarWin);

                    SearchResultsMenuWindow searchResultsMenuWindow(curWin);
                    WINDOW *searchResultMenuWin = searchResultsMenuWindow.drawWindow(rows / 2 + rows / 4, cols / 2 + cols / 4, 5, cols / 8, 4, 4);
                    wrefresh(searchResultMenuWin);

                    int cursorOffset = cols / 8 + 2;
                    while (true) {
                        curs_set(1);
                        wmove(stdscr, 3, cursorOffset + searchBarWindow.getSearchText().length());
                        searchBarCodes curSbCode = searchBarWindow.handleKeyEvent();
                        curs_set(0);
                        if (curSbCode == searchBarCodes::quit) break;
                        string searchText;
                        bool isRequestRequired = true;

                        if (curSbCode == searchBarCodes::textTyped) {
                            isRequestRequired = true;
                            searchText = searchBarWindow.getSearchText();
                        }

                        while (true) {
                            menuCodes curCode = searchResultsMenuWindow.handleKeyEvent(isRequestRequired, searchText);
                            if (curCode == menuCodes::quit) break;
                            if (curCode == menuCodes::ok)
                                wrefresh(searchResultMenuWin);
                            isRequestRequired = false;
                        }
                    }

                    return menuCodes::refreshWin;
                }
            }
                break;

            case 'q' :
                return menuCodes::quit;

            default:
                return menuCodes::ok;
        }
    }

    return menuCodes::ok;
}

bool MainMenuWindow::getRefreshCodeSnippetStatus() const {
    return refreshCodeSnippetStatus;
}