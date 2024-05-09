#pragma once

#include "menuWindow.hpp"
#include "../searchBarWindow.hpp"
#include "../../helpers/requests/questionListRequest.hpp"

#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;
using std::stringstream;
using std::left;
using std::setw;

class SearchResultsMenuWindow : public MenuWindow {
    vector<string> searchTasks(string &searchText);
    const int idWidth = 6;// 8
    const int titleWidth = 54; //50
    const int difficultyWidth = 15;//10
    const int statusWidth = 11;
    const int paidOnlyWidth = 10;

public:
    explicit SearchResultsMenuWindow(WINDOW *parentWin);

    WINDOW *drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) override;

    void refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) override;

    menuCodes handleKeyEvent(bool isRequestRequired, string searchText);
};