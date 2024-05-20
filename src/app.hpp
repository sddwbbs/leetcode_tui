#pragma once

#include "helpers/config.hpp"
#include "windows/mainWindow.hpp"
#include "windows/menus/menuWindow.hpp"
#include "windows/textWindow.hpp"
#include "helpers/task.hpp"
#include "windows/menus/mainMenuWindow.hpp"

#include <ncurses.h>
#include <cstdlib>
#include <pqxx/pqxx>
#include <iostream>
#include <clocale>

class App {
public:
    static void initColors();
    static void startApp();
};