#include "app.hpp"

#include "mainMenuWindow.hpp"
#include "mainWindow.hpp"

void App::startApp() {
    initscr();
    start_color();

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    WINDOW* mainWin = MainWindow::drawWindow(rows, cols, 0, 0);
    MainMenuWindow mainMenuWindow(stdscr, {"Open Nvim        ", "Open Daily Task  ", "Open Task List   "});
    WINDOW* mainMenuWin = mainMenuWindow.drawWindow(8, 30, rows / 2 - 4, cols / 2 - 15);

    wrefresh(mainWin);
    wrefresh(mainMenuWin);

    noecho();
    curs_set(0);

    Task* task = new Task();
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
}