#pragma once

#include "windows/mainWindow.hpp"
#include "windows/menus/menuWindow.hpp"
#include "windows/textWindow.hpp"
#include "helpers/task.hpp"

#include <ncurses.h>
#include <cstdlib>

class App {
public:
    static void startApp();
};