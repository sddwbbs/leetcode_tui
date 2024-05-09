#include "searchResultsMenuWindow.hpp"

SearchResultsMenuWindow::SearchResultsMenuWindow(WINDOW *parentWin)
        : MenuWindow(parentWin) {
    string emptyStr;
    menuItems = searchTasks(emptyStr);
    menuSize = menuItems.size();
}

vector<string> SearchResultsMenuWindow::searchTasks(string &searchText) {
    vector<json> questionList = QuestionListRequest::getQuestionList(searchText);
    vector<string> items;

    int i = 0;
    for (const auto& question : questionList) {
        if (i < 10) {
            string questionStr = question["frontendQuestionId"].dump() + question["title"].dump() + question["difficulty"].dump();
            items.push_back(questionStr);
        }
        else break;
        ++i;
    }

    return items;
}

menuCodes SearchResultsMenuWindow::handleKeyEvent(bool isRequestRequired, string searchText) {
    if (isRequestRequired) {
        menuItems = searchTasks(searchText);
        if (menuItems.empty()) {
            menuSize = 0;
            wclear(curWin);
            refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
            mvwprintw(curWin, 3, 4, "%s", "Nothing found");
            wrefresh(curWin);
            return menuCodes::quit;
        }
        menuSize = menuItems.size();
        refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
        wrefresh(curWin);
    }
    char ch;

    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(4, 4);
                return menuCodes::ok;
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(4, 4);
                return menuCodes::ok;
            }

            case '\n' : {
                //what happens if user select task
            }

            case 'q' : {
                return menuCodes::quit;
            }

            default:
                return menuCodes::ok;
        }
    }

    return menuCodes::ok;
}