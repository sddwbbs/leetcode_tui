#include "searchResultsMenuWindow.hpp"

SearchResultsMenuWindow::SearchResultsMenuWindow(WINDOW *parentWin, const int menuItemsLimit)
        : MenuWindow(parentWin), menuItemsLimit(menuItemsLimit) {
    string emptyStr;
    menuItems = searchTasks(emptyStr);
    menuSize = menuItems.size();
}

vector<string> SearchResultsMenuWindow::searchTasks(string &searchText) {
    vector<json> questionList = QuestionListRequest::getQuestionList(searchText);
    vector<string> items;

    int i = 0;
    for (const auto &question: questionList) {
        if (i < menuItemsLimit) {
            string frontendId = question["frontendQuestionId"];
            titleSlugMap[std::stoi(frontendId)] = question["titleSlug"];
            string title = question["title"];
            string difficulty = question["difficulty"];
            string status;
            if (!question["status"].is_null()) {
                if (question["status"] == "ac") status = "Solved";
                if (question["status"] == "notac") status = "Attempted";
            }

            string paidOnly;
            if (question["paidOnly"] == true)
                paidOnly = "x";

            stringstream ss;
            ss << std::setw(idWidth) << std::left << frontendId << " "
               << std::setw(titleWidth) << std::left << title << " "
               << std::setw(difficultyWidth) << difficulty << " "
               << std::setw(statusWidth) << status << " "
               << std::setw(paidOnlyWidth) << paidOnly;
            string questionStr = ss.str();
            items.push_back(questionStr);
        } else break;
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

    curWin = newwin(rows, cols, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    mvwprintw(curWin, 1, colsPadding, "%-*s%s%-*s%s%-*s%s%-*s%s%-*s",
              idWidth, "ID",
              " ",
              titleWidth, "TITLE",
              " ",
              difficultyWidth, "DIFFICULTY",
              " ",
              statusWidth, "STATUS",
              " ",
              paidOnlyWidth, "PAID ONLY");

    mvwhline(curWin, 2, colsPadding, ACS_HLINE, cols - 2 * colsPadding);

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + 1 + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 1 + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
        }
    }

    mvwvline(curWin, 1, idWidth + 1, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + 2, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + difficultyWidth + 3, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + difficultyWidth + statusWidth + 4, ACS_VLINE, rows - 2);

    return curWin;
}

void SearchResultsMenuWindow::refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding,
                                            Context context) {
    rows = _rows, cols = _cols, x = _x, y = _y;
    rowsPadding = _rowsPadding, colsPadding = _colsPadding;
    wresize(curWin, rows, cols);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, 0, 0);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(stdscr, COLOR_PAIR(2));
    int parentRows, parendCols;
    getmaxyx(stdscr, parentRows, parendCols);
    mvwprintw(stdscr, parentRows - 2, 3, "%s",
              "Press 'r' to read the task | 'o' to open it in nvim | 'c' to refresh code snippet");
    wattroff(stdscr, COLOR_PAIR(2));

    if (context == Context::nothingFound) {
        wattron(stdscr, COLOR_PAIR(2));
        string emptyLine(81, ' ');
        mvwprintw(stdscr, parentRows - 2, 3, "%s", emptyLine.c_str());
        wattroff(stdscr, COLOR_PAIR(2));
        wrefresh(curWin);
        return;
    }

    mvwprintw(curWin, 1, colsPadding, "%-*s%s%-*s%s%-*s%s%-*s%s%-*s",
              idWidth, "ID",
              " ",
              titleWidth, "TITLE",
              " ",
              difficultyWidth, "DIFFICULTY",
              " ",
              statusWidth, "STATUS",
              " ",
              paidOnlyWidth, "PAID ONLY");

    mvwhline(curWin, 2, colsPadding, ACS_HLINE, cols - 2 * colsPadding);

    for (int i = 0; i < menuSize; ++i) {
        if (i == curItem) {
            wattron(curWin, COLOR_PAIR(5));
            mvwprintw(curWin, i + 1 + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
            wattroff(curWin, COLOR_PAIR(5));
        } else {
            mvwprintw(curWin, i + 1 + rowsPadding, colsPadding, "%s", menuItems[i].c_str());
        }
    }

    mvwvline(curWin, 1, idWidth + 1, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + 2, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + difficultyWidth + 3, ACS_VLINE, rows - 2);
    mvwvline(curWin, 1, idWidth + titleWidth + difficultyWidth + statusWidth + 4, ACS_VLINE, rows - 2);

    wrefresh(curWin);
}

menuCodes SearchResultsMenuWindow::handleKeyEvent(bool isRequestRequired, string searchText, Task *task) {
    if (isRequestRequired) {
        menuItems = searchTasks(searchText);
        curItem = 0;
        if (menuItems.empty()) {
            menuSize = 0;
            wclear(curWin);
            refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::nothingFound);
            mvwprintw(curWin, 3, 4, "%s", "NOTHING FOUND");
            wrefresh(curWin);
            return menuCodes::quit;
        }
        menuSize = menuItems.size();
        refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
    }

    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) {
                    menuUp(rowsPadding, colsPadding);
                    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                }
                break;
            }

            case 'j' : {
                if (curItem < menuSize - 1) {
                    menuDown(rowsPadding, colsPadding);
                    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                }
                break;
            }

            case 'r' : {
                int curItem = getCurItem();
                string frontendId = menuItems[curItem].substr(0, menuItems[curItem].find(' '));
                string titleSlug = titleSlugMap.find(std::stoi(frontendId))->second;
                TextWindow taskTextWindow(task->getSingleTask(titleSlug).title, task->getSingleTask(titleSlug).content);
                WINDOW *taskTextWin = taskTextWindow.drawWindow(rows - 2, cols / 2 + cols / 8, x + 1, y + cols / 2 - (cols / 2 + cols / 8) / 2);
                wrefresh(taskTextWin);

                taskTextWindow.handleKeyEvent();

                refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                return menuCodes::ok;
            }

            case 'o' : {
                if (refreshCodeSnippetStatus || getCurItem() != selectedItem) {
                    selectedItem = getCurItem();
                    std::ofstream myFileOutput;
                    string selectedLang;
                    string codeSnippet;
                    int selectedLangIndex = -1;

                    LanguageMenuWindow languageMenuWindow(curWin);
                    WINDOW *languageMenuWin = languageMenuWindow.drawWindow(rows - 2, cols / 4,
                                                                            x + 1, y + cols / 2 - (cols / 8), 2,5);
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

                    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);

                    if (selectedLangIndex == -1)
                        return menuCodes::refreshWin;
                    selectedLang = lang[selectedLangIndex];
                    auto pos = languageMenuWindow.langExtMap.find(selectedLang);
                    if (pos != languageMenuWindow.langExtMap.end())
                        langExt = pos->second;

                    int curItem = getCurItem();
                    string frontendId = menuItems[curItem].substr(0, menuItems[curItem].find(' '));
                    string titleSlug = titleSlugMap.find(std::stoi(frontendId))->second;

                    for (const auto &item: task->getSingleTask(titleSlug).codeSnippets) {
                        if (item["lang"] == selectedLang) {
                            codeSnippet = item["code"];
                            break;
                        }
                    }

                    myFileOutput.open("singleTask." + langExt);
                    myFileOutput << codeSnippet;
                    myFileOutput.close();
                    refreshCodeSnippetStatus = false;
                }

                string command = "nvim singleTask." + langExt;
                system(command.c_str());
                endwin();
                initscr();

                refreshWindow(rows, cols, x, y, rowsPadding, colsPadding, Context::standard);
                return menuCodes::ok;
            }

            case '\n' : {
                // Handle selection, for example:
                mvwprintw(curWin, menuSize + 2, 4, "Selected: %s", menuItems[curItem].c_str());
                wrefresh(curWin);
                // Return a code or perform an action based on the selection
                break;
            }

            case 'q' : {
                return menuCodes::quit;
            }

            default:
                break;
        }
    }

    return menuCodes::ok;
}
