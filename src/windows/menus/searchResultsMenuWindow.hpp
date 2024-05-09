#pragma once

#include "menuWindow.hpp"
#include "../searchBarWindow.hpp"
#include "../../helpers/requests/questionListRequest.hpp"

#include <nlohmann/json.hpp>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;
using std::stringstream;
using std::left;
using std::setw;

class SearchResultsMenuWindow : public MenuWindow {
    vector<string> searchTasks(string &searchText);

public:
    explicit SearchResultsMenuWindow(WINDOW *parentWin);

    menuCodes handleKeyEvent(bool isRequestRequired, string searchText);
};