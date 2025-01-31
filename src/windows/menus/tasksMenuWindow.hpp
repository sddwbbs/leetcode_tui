#pragma once

#include "menuWindow.hpp"
#include "../searchBarWindow.hpp"
#include "../../helpers/requests/questionListRequest.hpp"
#include "../textWindow.hpp"
#include "languageMenuWindow.hpp"
#include "launchMenuWindow.hpp"

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
    standard,
    scroll
};

class SearchResultsMenuWindow final : public MenuWindow {
    vector<string> getTasks(string &searchText, bool doScroll);

    unordered_map<int, string> *titleSlugMap;
    vector<json> *questionList;
    string emptyStr;
    bool refreshCodeSnippetStatus = true;
    int selectedItem = -1;
    string langExt;
    const int idWidth = TOTAL_COLS / 20;
    const int titleWidth = TOTAL_COLS / 2 + TOTAL_COLS / 20;
    const int statusWidth = TOTAL_COLS / 5;
    const int paidOnlyWidth = TOTAL_COLS / 20;

public:
    SearchResultsMenuWindow() = delete;

    ~SearchResultsMenuWindow() override;

    explicit SearchResultsMenuWindow(WINDOW *grandParentWin);

    WINDOW *drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) override;

    void refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding, Context);

    menuCodes handleKeyEvent(bool isRequestRequired, string searchText, Task *task);
};