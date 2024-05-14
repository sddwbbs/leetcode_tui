#pragma once

#include "menuWindow.hpp"
#include "../searchBarWindow.hpp"
#include "../../helpers/requests/questionListRequest.hpp"
#include "../textWindow.hpp"
#include "languageMenuWindow.hpp"

#include <nlohmann/json.hpp>
#include <sstream>
#include <unordered_map>

using json = nlohmann::json;
using std::stringstream;
using std::left;
using std::setw;
using std::unordered_map;

enum class Context : int {
    nothingFound,
    standard
};

class SearchResultsMenuWindow : public MenuWindow {
    vector<string> searchTasks(string &searchText);

    unordered_map<int, string> titleSlugMap;
    bool refreshCodeSnippetStatus = true;
    int selectedItem = -1;
    string langExt;
    const int idWidth = 6;// 8
    const int titleWidth = 54; //50
    const int difficultyWidth = 15;//10
    const int statusWidth = 11;
    const int paidOnlyWidth = 10;
    const int menuItemsLimit = 0;

public:
    SearchResultsMenuWindow() = default;

    explicit SearchResultsMenuWindow(WINDOW *grandParentWin, int menuItemsLimit);

    WINDOW *drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) override;

    void refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding, Context);

    menuCodes handleKeyEvent(bool isRequestRequired, string searchText, Task *task);
};