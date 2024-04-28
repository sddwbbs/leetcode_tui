#pragma once

#include "menuWindow.hpp"
#include "../textWindow.hpp"
#include "../../helpers/task.hpp"
#include "launchMenuWindow.hpp"
#include "languageMenuWindow.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using nlohmann::json;

class MainMenuWindow : public MenuWindow {
    bool refreshCodeSnippetStatus = true;

public:
    explicit MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    int handleKeyEvent(Task *task) override;

    [[nodiscard]] bool getRefreshCodeSnippetStatus() const;
};