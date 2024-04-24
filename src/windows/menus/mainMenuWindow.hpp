#pragma once

#include "menuWindow.hpp"
#include "../textWindow.hpp"
#include "../../helpers/task.hpp"
#include "launchMenuWindow.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using nlohmann::json;

class MainMenuWindow : public MenuWindow {
    bool refreshCodeSnippetStatus = true;

public:
    explicit MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    WINDOW *drawWindow(int row, int col, int x, int y) override;

    void refreshWindow(int row, int col, int x, int y) override;

    int handleKeyEvent(Task *task) override;

    [[nodiscard]] bool getRefreshCodeSnippetStatus() const;
};