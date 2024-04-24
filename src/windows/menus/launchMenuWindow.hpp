#pragma once

#include "menuWindow.hpp"
#include "../textWindow.hpp"

class LaunchMenuWindow : public MenuWindow {
public:
    explicit LaunchMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    WINDOW *drawWindow(int row, int col, int x, int y) override;

    void refreshWindow(int row, int col, int x, int y) override;

    int handleKeyEvent(Task *task) override;
};