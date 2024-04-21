#include <pqxx/pqxx>
#include <iostream>

#include "app.hpp"
#include "windows/mainMenuWindow.hpp"
#include "windows/mainWindow.hpp"

void App::startApp() {
    initscr();
    start_color();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    try {
        pqxx::connection conn("dbname=leetcode_tui user=postgres password=8080 hostaddr=127.0.0.1 port=5432");

        WINDOW *mainWin = MainWindow::drawWindow(rows, cols, 0, 0);
        MainMenuWindow mainMenuWindow(stdscr, {"Open Nvim        ", "Open Daily Task  ", "Open Task List   "});
        WINDOW *mainMenuWin = mainMenuWindow.drawWindow(8, 30, rows / 2 - 4, cols / 2 - 15);

        wrefresh(mainWin);
        wrefresh(mainMenuWin);

        noecho();
        curs_set(0);

        Task *task = new Task(conn);
        while (true) {
            int curCode = mainMenuWindow.handleKeyEvent(task);
            if (curCode == static_cast<int>(menuCodes::quit)) break;
            if (curCode == static_cast<int>(menuCodes::refreshWin)) {
                MainWindow::refreshWindow(rows, cols, 0, 0);
                mainMenuWindow.refreshWindow(8, 30, rows / 2 - 4, cols / 2 - 15);
                wrefresh(mainWin);
                wrefresh(mainMenuWin);
            }
        }

        delete task;
        endwin();

    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return;
    }
}