#pragma once

#include "menuWindow.hpp"
#include "textWindow.hpp"
#include "helpers/task.hpp"

class MainMenuWindow : public MenuWindow {
public:
    explicit MainMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    WINDOW *drawWindow(int row, int col, int x, int y) override;

    void refreshWindow(int row, int col, int x, int y) override;

    int handleKeyEvent(Task *task) override;
};