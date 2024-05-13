#include <pqxx/pqxx>
#include <iostream>
#include <clocale>

#include "app.hpp"
#include "windows/menus/mainMenuWindow.hpp"
#include "windows/mainWindow.hpp"

void App::startApp() {
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    try {
        pqxx::connection conn("dbname=leetcode_tui user=postgres password=8080 hostaddr=127.0.0.1 port=5432");

        WINDOW *mainWin = MainWindow::drawWindow(rows, cols, 0, 0);
        MainMenuWindow mainMenuWindow(mainWin, {"Open Nvim        ", "Open Daily Task  ", "Open Tasks List  ", "\U0001F50D Search        "});
        WINDOW *mainMenuWin = mainMenuWindow.drawWindow(8, 30, rows / 2 - 4, cols / 2 - 15, 2, 5);

        wrefresh(mainWin);
        wrefresh(mainMenuWin);

        noecho();
        curs_set(0);

        Task *task = new Task(conn);
        while (true) {
            menuCodes curCode = mainMenuWindow.handleKeyEvent(task);
            if (curCode == menuCodes::quit) break;
            if (curCode == menuCodes::refreshWin) {
                MainWindow::refreshWindow(rows, cols, 0, 0);
                mainMenuWindow.refreshWindow(8, 30, rows / 2 - 4, cols / 2 - 15, 2, 5);
                wattron(mainWin, COLOR_PAIR(2));
                if (mainMenuWindow.getCurItem() == 1) {
                    mvwprintw(mainWin, rows - 2, 3,
                              "Press 'r' to read the task | 'o' to open it in nvim | 'c' to refresh code snippet");
                    if (!mainMenuWindow.getRefreshCodeSnippetStatus())
                        mvwprintw(mainWin, rows / 2 - 1, cols / 2 + 16,
                                  "'Enter' to Run or Submit");
                }
                wattroff(mainWin, COLOR_PAIR(2));
                wnoutrefresh(mainWin);
                wnoutrefresh(mainMenuWin);
                refresh();
            }
        }

        delete task;
        endwin();

    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return;
    }
}