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
#include <fstream>
#include <sstream>

class App {
public:
    static bool isDatabaseExists(pqxx::connection &conn, const string &dbName);

    static string readScript(const string &fileName);

    static void initColors();

    static void startApp();
};
