#pragma once

#include "helpers/constants.hpp"
#include "helpers/config.hpp"
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
#include <thread>
#include <chrono>

class App {
    static std::shared_ptr<pqxx::connection> conn;

    static std::shared_ptr<Task> task;

    static Task *taskPtr;

public:
    static bool isDatabaseExists(pqxx::connection &conn, const string &dbName);

    static bool isTableExists(pqxx::connection &conn, const string &tableName);

    static string readScript(const string &fileName);

    static void initColors();

    static void startApp();
};