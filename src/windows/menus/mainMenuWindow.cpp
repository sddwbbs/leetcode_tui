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
                    string emptyStr = "Wait";
                    string loadingStr = "Loading...";
                    TextWindow loadingWindow(emptyStr, loadingStr);
                    WINDOW *loadingWin = loadingWindow.drawWindow(rows / 2 + rows / 4, cols / 2, rows / 6,
                                                                  cols / 2 - (cols / 4), 4);
                    wrefresh(loadingWin);

                    int boxColorPair = 4;
                    string title = task->getDailyTask().title;
                    string difficulty = task->getDailyTask().difficulty;
                    if (difficulty == "Easy")
                        boxColorPair = 2;
                    if (difficulty == "Medium")
                        boxColorPair = 7;
                    if (difficulty == "Hard")
                        boxColorPair = 8;
                    string content = task->getDailyTask().content;
                    TextWindow dailyTaskTextWindow(title, content);

                    werase(loadingWin);
                    WINDOW *dailyTaskTextWin = dailyTaskTextWindow.drawWindow(rows / 2 + rows / 4, cols / 2, rows / 6,
                                                                              cols / 2 - (cols / 4), boxColorPair);
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
                        WINDOW *languageMenuWin = languageMenuWindow.drawWindow(rows / 2 + rows / 4, cols / 4,
                                                                                rows / 6, cols / 2 - (cols / 8), 2,5);
                        wrefresh(languageMenuWin);

                        while (true) {
                            menuCodes curCode = languageMenuWindow.handleKeyEvent();
                            if (curCode == menuCodes::itemSelected) {
                                selectedLangIndex = languageMenuWindow.getCurItem();
                                break;
                            }
                            if (curCode == menuCodes::quit) break;
                            if (curCode == menuCodes::refreshWin)
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
                        menuCodes curCode = launchMenuWindow.handleKeyEvent(task, true, langExt);
                        if (curCode == menuCodes::quit) break;
                        if (curCode == menuCodes::refreshWin)
                            wrefresh(launchMenuWin);
                    }

                    return menuCodes::refreshWin;
                }

                if (curItem == 3) {
                    MainWindow::clearWindowContent();
                    wrefresh(parentWin);

                    SearchBarWindow searchBarWindow;
                    WINDOW *searchBarWin = searchBarWindow.drawWindow(3, cols / 2 + cols / 4 + cols / 8, 2, (cols - (cols / 2 + cols / 4 + cols / 8)) / 2);
                    wrefresh(searchBarWin);

                    SearchResultsMenuWindow searchResultsMenuWindow(curWin, (rows / 2 + rows / 4) - 4);
                    WINDOW *searchResultMenuWin = searchResultsMenuWindow.drawWindow(rows / 2 + rows / 4, cols / 2 + cols / 4 + cols / 8, 5, (cols - (cols / 2 + cols / 4 + cols / 8)) / 2, 2, 2);
                    wrefresh(searchResultMenuWin);

                    int cursorOffset = (cols - (cols / 2 + cols / 4 + cols / 8)) / 2 + 2;
                    while (true) {
                        curs_set(1);
                        wmove(stdscr, 3, cursorOffset + searchBarWindow.getSearchText().length());
                        searchBarCodes curSbCode = searchBarWindow.handleKeyEvent();
                        curs_set(0);

                        if (curSbCode == searchBarCodes::quit) break;
                        string searchText;
                        bool isRequestRequired = true;

                        if (curSbCode == searchBarCodes::ok)
                            isRequestRequired = false;

                        if (curSbCode == searchBarCodes::textTyped) {
                            isRequestRequired = true;
                            searchText = searchBarWindow.getSearchText();
                        }

                        while (true) {
                            menuCodes curCode = searchResultsMenuWindow.handleKeyEvent(isRequestRequired, searchText, task);
                            if (curCode == menuCodes::quit) break;
                            if (curCode == menuCodes::refreshWin)
                                wnoutrefresh(searchResultMenuWin);
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