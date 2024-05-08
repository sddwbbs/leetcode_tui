#pragma once

#include "menuWindow.hpp"
#include "../searchBarWindow.hpp"

class SearchResultsMenuWindow : public MenuWindow {
    vector<string> searchTasks();
    string *searchText = nullptr;

public:
    explicit SearchResultsMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    int handleKeyEvent(string *searchText);
};