#pragma once

#include "menuWindow.hpp"
#include "../mainWindow.hpp"
#include "../textWindow.hpp"
#include "../../helpers/task.hpp"
#include "launchMenuWindow.hpp"
#include "languageMenuWindow.hpp"
#include "../searchBarWindow.hpp"
#include "searchResultsMenu.hpp"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace fs = std::filesystem;

class MainMenuWindow : public MenuWindow {
    bool refreshCodeSnippetStatus = true;
    string langExt;
    int rows = 0;
    int cols = 0;

public:
    explicit MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    int handleKeyEvent(Task *task);

    [[nodiscard]] bool getRefreshCodeSnippetStatus() const;
};