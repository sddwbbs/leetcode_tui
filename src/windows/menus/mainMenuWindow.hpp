#pragma once

#include "menuWindow.hpp"
#include "../textWindow.hpp"
#include "../../helpers/task.hpp"
#include "launchMenuWindow.hpp"
#include "languageMenuWindow.hpp"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace fs = std::filesystem;

class MainMenuWindow : public MenuWindow {
    bool refreshCodeSnippetStatus = true;
    string langExt;

public:
    explicit MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    int handleKeyEvent(Task *task) override;

    [[nodiscard]] bool getRefreshCodeSnippetStatus() const;
};