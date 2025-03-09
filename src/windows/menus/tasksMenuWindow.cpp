#include "tasksMenuWindow.hpp"

SearchResultsMenuWindow::SearchResultsMenuWindow(WINDOW *grandParentWin)
    : MenuWindow(grandParentWin)
      , titleSlugMap(new(unordered_map<int, string>))
      , questionList(new(vector<json>)) {
}

SearchResultsMenuWindow::~SearchResultsMenuWindow() {
    delete titleSlugMap;
    delete questionList;
}

vector<string> SearchResultsMenuWindow::getTasks(string &searchText, const bool doScroll) {
    if (!titleSlugMap->empty())
        titleSlugMap->clear();

    if (doScroll == false) *questionList = QuestionListRequest::getQuestionList(searchText);
    vector<string> items;

    pageLimit = static_cast<int>(questionList->size()) / (rows - 4);
    if (static_cast<int>(questionList->size()) % (rows - 4) != 0)
        ++pageLimit;

    int i = 0;
    for (const auto &question: *questionList) {
        if (i >= beginOfDisplayedItemsIdx && i < beginOfDisplayedItemsIdx + rows - 4) {
            string frontendId = question["frontendQuestionId"];
            (*titleSlugMap)[std::stoi(frontendId)] = question["titleSlug"];

            string title = question["title"];
            string status;
            if (!question["status"].is_null()) {
                if (question["status"] == "ac") status = "Solved";
                if (question["status"] == "notac") status = "Attempted";
            }

            string paidOnly;
            paidOnly = question["paidOnly"] ? "  \U0001F512   " : "   ";

            stringstream ss;
            ss << std::setw(idWidth) << std::left << frontendId << " "
                    << std::setw(titleWidth) << std::left << title << " "
                    << std::setw(statusWidth) << status << " "
                    << std::setw(paidOnlyWidth) << paidOnly;
            string questionStr = ss.str();
            items.push_back(questionStr);
        }
        ++i;
    }

    return items;
}

WINDOW *SearchResultsMenuWindow::drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);

    curWin = newwin(rows, cols, y, x);
    wnoutrefresh(curWin);

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(0));

    string emptyStr;
    menuItems = getTasks(emptyStr, false);

    mvwprintw(curWin, 1, colsPadding, "%-*s%s%-*s%s%-*s%s%-*s",
              idWidth, "ID",
              " ",
              titleWidth, "TITLE",
              " ",
              statusWidth, "STATUS",
              " ",
              paidOnlyWidth, "PAID ONLY");

    mvwhline(curWin, 2, colsPadding, ACS_HLINE, cols - 2 * colsPadding);

    for (int curElemIdx = 0; curElemIdx < menuItems.size(); ++curElemIdx) {
        if (curElemIdx == selectedItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
        }
    }

    mvwvline(curWin, 1, idWidth + 1, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + 2, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + statusWidth + 3, ACS_VLINE, rows - 2);

    return curWin;
}

void SearchResultsMenuWindow::refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding,
                                            const Context context) {
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    wresize(curWin, rows, cols);
    mvwin(curWin, y, x);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(0));

    wattron(stdscr, COLOR_PAIR(0));
    int parentRows, parendCols;
    getmaxyx(stdscr, parentRows, parendCols);
    const string emptyLine(parendCols - 4, ' ');
    mvwprintw(stdscr, parentRows - 2, 3, "%s", emptyLine.c_str());
    if (selectedItemIdx != selectedItem) {
        mvwprintw(stdscr, parentRows - 2, 3, "%s",
                  "Press 'r' to read the task | 'o' to open it in nvim ");
    } else {
        mvwprintw(stdscr, parentRows - 2, 3, "%s",
                  "Press 'r' to read the task | 'o' to open it in nvim | 'c' to refresh code snippet | 'Enter' to Run or Submit");
    }
    wattroff(stdscr, COLOR_PAIR(0));

    if (context == Context::nothingFound) {
        wattron(stdscr, COLOR_PAIR(0));
        mvwprintw(stdscr, parentRows - 2, 3, "%s", emptyLine.c_str());
        wattroff(stdscr, COLOR_PAIR(0));
        wnoutrefresh(curWin);
        return;
    }

    if (context == Context::scroll) {
        string emptyStr;
        menuItems = getTasks(emptyStr, true);
    }

    mvwprintw(curWin, 1, colsPadding, "%-*s%s%-*s%s%-*s%s%-*s",
              idWidth, "ID",
              " ",
              titleWidth, "TITLE",
              " ",
              statusWidth, "STATUS",
              " ",
              paidOnlyWidth, "PAID ONLY");

    mvwhline(curWin, 2, colsPadding, ACS_HLINE, cols - 2 * colsPadding);

    for (int curElemIdx = 0; curElemIdx < menuItems.size(); ++curElemIdx) {
        if (curElemIdx == selectedItem) {
            wattron(curWin, COLOR_PAIR(6));
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
            wattroff(curWin, COLOR_PAIR(6));
        } else if (curElemIdx == selectedItemIdx) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, curElemIdx + rowsPadding, colsPadding, "%s",
                      menuItems[curElemIdx].c_str());
        }
    }

    mvwvline(curWin, 1, idWidth + 1, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + 2, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + statusWidth + 3, ACS_VLINE, rows - 2);

    wnoutrefresh(curWin);
    doupdate();
}

menuCodes SearchResultsMenuWindow::handleKeyEvent(bool isRequestRequired, string searchText, Task *task) {
    if (isRequestRequired) {
        menuItems = getTasks(searchText, false);
        selectedItemIdx = 0;
        if (menuItems.empty()) {
            wclear(curWin);
            refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::nothingFound);
            mvwprintw(curWin, 3, 4, "%s", "NOTHING FOUND \U0001F61E");
            wnoutrefresh(curWin);
            return menuCodes::quit;
        }
        refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
    }

    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k': {
                if (selectedItemIdx == 0) {
                    scrollUp();
                    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::scroll);
                    break;
                }
                else
                    menuUp(rowsPadding, colsPadding);
                refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
            }
            break;

            case 'j': {
                if (selectedItemIdx == menuItems.size() - 1) {
                    scrollDown();
                    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::scroll);
                    break;
                }
                else
                    menuDown(rowsPadding, colsPadding);
                refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
            }
            break;

            case 'r': {
                string emptyStr = "Wait";
                string loadingStr = "Loading...";
                TextWindow loadingWindow(emptyStr, loadingStr);
                WINDOW *loadingWin = loadingWindow.drawWindow(rows - 2, cols / 2 + cols / 8, x + 1,
                                                              y + cols / 2 - (cols / 2 + cols / 8) / 2, 3);
                wrefresh(loadingWin);

                //TODO надо что-то получше сюда
                string frontendId = menuItems[selectedItemIdx].substr(0, menuItems[selectedItemIdx].find(' '));
                string titleSlug = titleSlugMap->find(std::stoi(frontendId))->second;

                if (menuItems[selectedItemIdx].find("\U0001F512") != string::npos) {
                    string content = "This task is paid only :(";
                    titleSlug += " \U0001F512";
                    TextWindow messageWindow(titleSlug, content);
                    WINDOW *taskTextWin = messageWindow.drawWindow(rows - 2, cols / 2 + cols / 8, x + 1,
                                                                    y + cols / 2 - (cols / 2 + cols / 8) / 2, 3);
                    wrefresh(taskTextWin);

                    messageWindow.handleKeyEvent();
                } else {
                    // TextWindow taskTextWindow(task->getSingleTask(titleSlug).title,
                    //                           task->getSingleTask(titleSlug).content);
                    // WINDOW *taskTextWin = taskTextWindow.drawWindow(rows - 2, cols / 2 + cols / 8, x + 1,
                    //                                                 y + cols / 2 - (cols / 2 + cols / 8) / 2, 3);
                    // wrefresh(taskTextWin);
                    //
                    // taskTextWindow.handleKeyEvent();
                    task->getSingleTask(titleSlug);
                    task->displayTask();
                    return menuCodes::refreshWin;
                }

                werase(loadingWin);

                refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                return menuCodes::refreshWin;
            }
            break;

            case 'o': {
                // if (refreshCodeSnippetStatus || getRelativeIdx() != selectedItem) {
                //     if (menuItems[absoluteItemIdx].find("\U0001F512") != string::npos) {
                //         string frontendId = menuItems[absoluteItemIdx].substr(0, menuItems[absoluteItemIdx].find(' '));
                //         string titleSlug = titleSlugMap->find(std::stoi(frontendId))->second;
                //         string content = "This task is paid only :(";
                //         titleSlug += " \U0001F512";
                //         TextWindow taskTextWindow(titleSlug, content);
                //         WINDOW *taskTextWin = taskTextWindow.drawWindow(rows - 2, cols / 2 + cols / 8, x + 1,
                //                                                         y + cols / 2 - (cols / 2 + cols / 8) / 2, 3);
                //         wrefresh(taskTextWin);
                //
                //         taskTextWindow.handleKeyEvent();
                //
                //         refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                //         return menuCodes::refreshWin;
                //     }
                //
                //     std::ofstream myFileOutput;
                //     string selectedLang;
                //     string codeSnippet;
                //
                //     //TODO: Make list of tasks
                //     LanguageMenuWindow languageMenuWindow(curWin, {"hello"});
                //     WINDOW *languageMenuWin = languageMenuWindow.drawWindow(rows - 2, cols / 4,
                //                                                             x + 1, y + cols / 2 - (cols / 8), 2, 5);
                //     wrefresh(languageMenuWin);
                //
                //     while (true) {
                //         menuCodes curCode = languageMenuWindow.handleKeyEvent();
                //         if (curCode == menuCodes::itemSelected) break;
                //         if (curCode == menuCodes::quit) {
                //             refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                //             return menuCodes::refreshWin;
                //         }
                //         if (curCode == menuCodes::refreshWin)
                //             wrefresh(languageMenuWin);
                //     }
                //
                //     selectedItem = getRelativeIdx();
                //     refreshCodeSnippetStatus = false;
                //
                //     refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                //
                //     selectedLang = "C++";
                //     auto pos = languageMenuWindow.langExtMap.find(selectedLang);
                //     if (pos != languageMenuWindow.langExtMap.end())
                //         langExt = pos->second;
                //
                //     string frontendId = menuItems[selectedItem].substr(0, menuItems[selectedItem].find(' '));
                //     string titleSlug = titleSlugMap->find(std::stoi(frontendId))->second;
                //
                //     for (const auto &item: task->getSingleTask(titleSlug).codeSnippets) {
                //         if (item["lang"] == selectedLang) {
                //             codeSnippet = item["code"];
                //             break;
                //         }
                //     }
                //
                //     myFileOutput.open("singleTask." + langExt);
                //     myFileOutput << codeSnippet;
                //     myFileOutput.close();
                // }
                //
                // string command = "nvim singleTask." + langExt;
                // system(command.c_str());
                // endwin();
                // initscr();
                //
                // refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                return menuCodes::refreshWin;
            }
            break;

            case 'c': {
                // if (!refreshCodeSnippetStatus && getRelativeIdx() == selectedItem) {
                //     selectedItem = -1;
                //     refreshCodeSnippetStatus = true;
                //     refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                // }
                return menuCodes::refreshWin;
            }
            break;

            case '\n': {
                // if (!refreshCodeSnippetStatus && getRelativeIdx() == selectedItem) {
                //     LaunchMenuWindow launchMenuWindow(curWin, {"  Run     ", "  Submit  "});
                //     WINDOW *launchMenuWin = launchMenuWindow.drawWindow(4, 12, x + getRelativeIdx() + 1,
                //                                                         cols / 2 + cols / 4, 1, 1);
                //     wrefresh(launchMenuWin);
                //
                //     while (true) {
                //         menuCodes curCode = launchMenuWindow.handleKeyEvent(task, false, langExt);
                //         if (curCode == menuCodes::quit) {
                //             refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                //             return menuCodes::refreshWin;
                //         }
                //         if (curCode == menuCodes::refreshWin)
                //             wrefresh(launchMenuWin);
                //     }
                // }
                return menuCodes::refreshWin;
            }
            break;

            case 'q': {
                return menuCodes::quit;
            }
            break;

            default: break;
        }
    }

    return menuCodes::ok;
}
