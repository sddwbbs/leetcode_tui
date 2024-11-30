#pragma once

#include "../../helpers/constants.hpp"
#include "menuWindow.hpp"
#include "../textWindow.hpp"
#include "../../helpers/task.hpp"
#include "launchMenuWindow.hpp"
#include "languageMenuWindow.hpp"
#include "../searchBarWindow.hpp"
#include "tasksMenuWindow.hpp"
#include "../../helpers/requests/questionListRequest.hpp"

#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

using nlohmann::json;

namespace fs = std::filesystem;

class MainMenuWindow final : public MenuWindow {
    bool refreshCodeSnippetStatus = true;
    string langExt;

public:
    explicit MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    WINDOW *drawWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) override;

    void refreshWindow(int _rows, int _cols, int _x, int _y, int _rowsPadding, int _colsPadding) override;

    menuCodes handleKeyEvent(Task *task);

    [[nodiscard]] bool getRefreshCodeSnippetStatus() const;
};