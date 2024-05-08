#pragma once

#include "menus/searchResultsMenuWindow.hpp"

#include <ncurses.h>
#include <string>

using std::string;

class SearchBarWindow {
    WINDOW *curWin = nullptr;
    string searchText;
    string emptyStr;
    int rows = 0;
    int cols = 0;
    int x = 0;
    int y = 0;

public:
    SearchBarWindow() = default;

    SearchBarWindow(const SearchBarWindow &) = delete;

    SearchBarWindow &operator=(const SearchBarWindow &) = delete;

    WINDOW *drawWindow(int rows, int cols, int x, int y);

    void refreshWindow(int rows, int cols, int x, int y);

    string &handleKeyEvent();

    [[nodiscard]] string getSearchText() const;
};