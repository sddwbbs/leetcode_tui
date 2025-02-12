#pragma once

#include "menus/tasksMenuWindow.hpp"

#include <ncurses.h>
#include <string>

enum class searchBarCodes : int {
    ok,
    quit,
    textTyped,
};

using std::string;

class SearchBarWindow {
    WINDOW *curWin = nullptr;
    string searchText;
    string prevSearchText;
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

    searchBarCodes handleKeyEvent();

    [[nodiscard]] string getSearchText() const;
};