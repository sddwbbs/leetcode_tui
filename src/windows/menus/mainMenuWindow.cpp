#include "mainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
    : MenuWindow(parentWin, menuItems) {
}

WINDOW *MainMenuWindow::drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;

    curWin = newwin(rows, cols, y, x);
    refresh();

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(0));

    wattron(curWin, COLOR_PAIR(0));
    mvwprintw(curWin, 0, 6, " %s ", "MENU");
    wattroff(curWin, COLOR_PAIR(0));

    wattron(curWin, A_BOLD | COLOR_PAIR(0));
    mvwprintw(curWin, 2, colsPadding, "%s", "Daily Task");
    wattroff(curWin, A_BOLD | COLOR_PAIR(0));

    mvwhline(curWin, 6, 1, 0, cols - 2);

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, (i == 0) ? i + rowsPadding : i + rowsPadding + 3,
                      colsPadding, (i == 0) ? "%s" : "- %s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, (i == 0) ? i + rowsPadding : i + rowsPadding + 3,
                      colsPadding, (i == 0) ? "%s" : "- %s", menuItems[i].c_str());
        }
    }

    return curWin;
}

void MainMenuWindow::refreshWindow(const int _rows, const int _cols, const int _x, const int _y, const int _rowsPadding,
                                   const int _colsPadding) {
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    wresize(curWin, rows, cols);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(0));

    wattron(curWin, COLOR_PAIR(0));
    mvwprintw(curWin, 0, 6, " %s ", "MENU");
    wattroff(curWin, COLOR_PAIR(0));

    wattron(curWin, A_BOLD | COLOR_PAIR(0));
    mvwprintw(curWin, 2, colsPadding, "%s", "Daily Task");
    wattroff(curWin, A_BOLD | COLOR_PAIR(0));

    mvwhline(curWin, 6, 1, 0, cols - 2);

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, (i == 0) ? i + rowsPadding : i + rowsPadding + 3,
                      colsPadding, (i == 0) ? "%s" : "- %s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, (i == 0) ? i + rowsPadding : i + rowsPadding + 3,
                      colsPadding, (i == 0) ? "%s" : "- %s", menuItems[i].c_str());
        }
    }
}

menuCodes MainMenuWindow::handleKeyEvent(Task *task) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k': {
                if (curItemIdx > 0) {
                    menuUp(2, 5);
                    return menuCodes::refreshWin;
                }
            }
            break;

            case 'j': {
                if (curItemIdx < menuSize - 1) {
                    menuDown(2, 5);
                    return menuCodes::refreshWin;
                }
            }
            break;

            case 'r': {
                if (curItemIdx == 0) {
                    endwin();
                    system("w3m /tmp/temp.html");
                    return menuCodes::refreshWin;
                }
            }
            break;

            case 'o': {
                if (curItemIdx == 0) {
                    if (refreshCodeSnippetStatus) {
                        std::ofstream myFileOutput;
                        string selectedLang;
                        string codeSnippet;
                        vector<string> langList;

                        json curTaskCodeSnippets = task->getDailyTask().codeSnippets;
                        for (const auto &item:curTaskCodeSnippets) {
                            langList.emplace_back(item["lang"]);
                        }

                        LanguageMenuWindow languageMenuWindow(curWin, langList);
                        WINDOW *languageMenuWin = languageMenuWindow.drawWindow(
                            26, TOTAL_COLS / 6,
                            TOTAL_COLS / 2 - TOTAL_COLS / 12, TOTAL_ROWS / 2 - TOTAL_ROWS / 4 - 2, 3, 5);
                        wrefresh(languageMenuWin);

                        while (true) {
                            menuCodes curCode = languageMenuWindow.handleKeyEvent();
                            if (curCode == menuCodes::itemSelected) {
                                selectedLang = languageMenuWindow.getCurItem();
                                break;
                            }
                            if (curCode == menuCodes::quit) break;
                            if (curCode == menuCodes::refreshWin)
                                wrefresh(languageMenuWin);
                        }

                        if (selectedLang.empty()) {
                            clear();
                            return menuCodes::refreshWin;
                        }

                        if (auto pos = languageMenuWindow.langExtMap.find(selectedLang);
                            pos != languageMenuWindow.langExtMap.end())
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

            case 'c': {
                if (curItemIdx == 1) {
                    refreshCodeSnippetStatus = true;
                    return menuCodes::refreshWin;
                }
            }
            break;

            case 10: {
                // Enter
                if (curItemIdx == 0 && !refreshCodeSnippetStatus) {
                    LaunchMenuWindow launchMenuWindow(curWin, {"  Run     ", "  Submit  "});
                    WINDOW *launchMenuWin = launchMenuWindow.drawWindow(4, 12, TOTAL_ROWS / 2, TOTAL_COLS / 2 + 18, 1,
                                                                        1);
                    wrefresh(launchMenuWin);

                    while (true) {
                        menuCodes curCode = launchMenuWindow.handleKeyEvent(task, true, langExt);
                        if (curCode == menuCodes::quit) break;
                        if (curCode == menuCodes::refreshWin)
                            wrefresh(launchMenuWin);
                    }

                    return menuCodes::refreshWin;
                }

                if (curItemIdx == 2) {
                    clear();
                    wrefresh(parentWin);

                    SearchBarWindow searchBarWindow;
                    WINDOW *searchBarWin = searchBarWindow.drawWindow(3, TOTAL_COLS - 10, 5, 3);
                    wrefresh(searchBarWin);

                    SearchResultsMenuWindow searchResultsMenuWindow(curWin, (TOTAL_ROWS / 2 + TOTAL_ROWS / 4) - 4);
                    WINDOW *searchResultMenuWin = searchResultsMenuWindow.drawWindow(
                        15, 60, 2, 7, 2, 2);
                    wrefresh(searchResultMenuWin);

                    int cursorOffset = (TOTAL_COLS - (TOTAL_COLS / 2 + TOTAL_COLS / 4 + TOTAL_COLS / 8)) / 2 + 2;
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
                            menuCodes curCode = searchResultsMenuWindow.handleKeyEvent(
                                isRequestRequired, searchText, task);
                            if (curCode == menuCodes::quit) break;
                            if (curCode == menuCodes::refreshWin)
                                wnoutrefresh(searchResultMenuWin);
                            isRequestRequired = false;
                        }
                    }

                    clear();
                    return menuCodes::refreshWin;
                }
            }
            break;

            case 'q':
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
