#include "searchResultsMenuWindow.hpp"

SearchResultsMenuWindow::SearchResultsMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems) {}

vector<string> SearchResultsMenuWindow::searchTasks() {
    vector<json> questionList = QuestionListRequest::getQuestionList(searchText);
    vector<string> items(10);

    int i = 0;
    for (const auto& question : questionList) {
        if (i < 10)
            items[i] = question["title"];
        else break;
        ++i;
    }

    return items;
}

int SearchResultsMenuWindow::handleKeyEvent(string *_searchText) {
    searchText = _searchText;
    menuItems = searchTasks();
    menuSize = menuItems.size();
    refreshWindow(rows, cols, x, y, rowsPadding, colsPadding);
    wrefresh(curWin);
    char ch;

    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(4, 4);
                return static_cast<int>(menuCodes::ok);
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(4, 4);
                return static_cast<int>(menuCodes::ok);
            }

            case 10 : {
            }

            case 'q' : {
                return static_cast<int>(menuCodes::quit);
            }

            default:
                return static_cast<int>(menuCodes::ok);
        }
    }

    return 1;
}

