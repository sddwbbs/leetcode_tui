#pragma once

#include "menuWindow.hpp"
#include "../textWindow.hpp"

class LaunchMenuWindow : public MenuWindow {
public:
    explicit LaunchMenuWindow(WINDOW *parentWin, const vector<string> &menuItems);

    int handleKeyEvent(Task *task);
};