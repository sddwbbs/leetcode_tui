#include "mainWindow.hpp"

WINDOW *MainWindow::curWin = nullptr;
int MainWindow::rows = 0;
int MainWindow::cols = 0;
int MainWindow::x = 0;
int MainWindow::y = 0;

WINDOW *MainWindow::drawWindow(int _rows, int _cols, int _x, int _y) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols;
    x = _x, y = _y;
    init_color(COLOR_BLACK, 82, 35, 72);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    curWin = newwin(rows, cols, x, y);
    refresh();

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 1, 10, "\n"
                             "                         $$\\                           $$\\      $$$$$$\\                  $$\\           \n"
                             "                         $$ |                          $$ |    $$  __$$\\                 $$ |          \n"
                             "                         $$ |      $$$$$$\\   $$$$$$\\ $$$$$$\\   $$ /  \\__| $$$$$$\\   $$$$$$$ | $$$$$$\\  \n"
                             "                         $$ |     $$  __$$\\ $$  __$$\\\\_$$  _|  $$ |      $$  __$$\\ $$  __$$ |$$  __$$\\ \n"
                             "                         $$ |     $$$$$$$$ |$$$$$$$$ | $$ |    $$ |      $$ /  $$ |$$ /  $$ |$$$$$$$$ |\n"
                             "                         $$ |     $$   ____|$$   ____| $$ |$$\\ $$ |  $$\\ $$ |  $$ |$$ |  $$ |$$   ____|\n"
                             "                         $$$$$$$$\\\\$$$$$$$\\ \\$$$$$$$\\  \\$$$$  |\\$$$$$$  |\\$$$$$$  |\\$$$$$$$ |\\$$$$$$$\\ \n"
                             "                         \\________|\\_______| \\_______|  \\____/  \\______/  \\______/  \\_______| \\_______|\n"
                             "                                                                                \n"
                             "                                                                                \n"
                             "                                                                                ");
    wattroff(curWin, COLOR_PAIR(1));

    wattron(curWin, COLOR_PAIR(2));
    box(curWin, x, y);
    wattroff(curWin, COLOR_PAIR(2));

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 0, cols / 2 - 7, " LeetCode TUI ");
    wattroff(curWin, COLOR_PAIR(1));


    return curWin;
}

void MainWindow::refreshWindow(int _rows, int _cols, int _x, int _y) {
    rows = _rows, cols = _cols;
    x = _x, y = _y;
    wresize(curWin, rows, cols);
    mvwin(curWin, x, y);
    werase(curWin);

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 1, 10, "\n"
                             "                         $$\\                           $$\\      $$$$$$\\                  $$\\           \n"
                             "                         $$ |                          $$ |    $$  __$$\\                 $$ |          \n"
                             "                         $$ |      $$$$$$\\   $$$$$$\\ $$$$$$\\   $$ /  \\__| $$$$$$\\   $$$$$$$ | $$$$$$\\  \n"
                             "                         $$ |     $$  __$$\\ $$  __$$\\\\_$$  _|  $$ |      $$  __$$\\ $$  __$$ |$$  __$$\\ \n"
                             "                         $$ |     $$$$$$$$ |$$$$$$$$ | $$ |    $$ |      $$ /  $$ |$$ /  $$ |$$$$$$$$ |\n"
                             "                         $$ |     $$   ____|$$   ____| $$ |$$\\ $$ |  $$\\ $$ |  $$ |$$ |  $$ |$$   ____|\n"
                             "                         $$$$$$$$\\\\$$$$$$$\\ \\$$$$$$$\\  \\$$$$  |\\$$$$$$  |\\$$$$$$  |\\$$$$$$$ |\\$$$$$$$\\ \n"
                             "                         \\________|\\_______| \\_______|  \\____/  \\______/  \\______/  \\_______| \\_______|\n"
                             "                                                                                \n"
                             "                                                                                \n"
                             "                                                                                ");
    wattroff(curWin, COLOR_PAIR(1));

    wattron(curWin, COLOR_PAIR(2));
    box(curWin, x, y);
    wattroff(curWin, COLOR_PAIR(2));

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 0, cols / 2 - 7, " LeetCode TUI ");
    wattroff(curWin, COLOR_PAIR(1));
}

void MainWindow::clearWindowContent() {
    werase(curWin);

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 1, 10, "\n"
                             "                         $$\\                           $$\\      $$$$$$\\                  $$\\           \n"
                             "                         $$ |                          $$ |    $$  __$$\\                 $$ |          \n"
                             "                         $$ |      $$$$$$\\   $$$$$$\\ $$$$$$\\   $$ /  \\__| $$$$$$\\   $$$$$$$ | $$$$$$\\  \n"
                             "                         $$ |     $$  __$$\\ $$  __$$\\\\_$$  _|  $$ |      $$  __$$\\ $$  __$$ |$$  __$$\\ \n"
                             "                         $$ |     $$$$$$$$ |$$$$$$$$ | $$ |    $$ |      $$ /  $$ |$$ /  $$ |$$$$$$$$ |\n"
                             "                         $$ |     $$   ____|$$   ____| $$ |$$\\ $$ |  $$\\ $$ |  $$ |$$ |  $$ |$$   ____|\n"
                             "                         $$$$$$$$\\\\$$$$$$$\\ \\$$$$$$$\\  \\$$$$  |\\$$$$$$  |\\$$$$$$  |\\$$$$$$$ |\\$$$$$$$\\ \n"
                             "                         \\________|\\_______| \\_______|  \\____/  \\______/  \\______/  \\_______| \\_______|\n"
                             "                                                                                \n"
                             "                                                                                \n"
                             "                                                                                ");
    wattroff(curWin, COLOR_PAIR(1));

    wattron(curWin, COLOR_PAIR(2));
    box(curWin, x, y);
    wattroff(curWin, COLOR_PAIR(2));

    wattron(curWin, COLOR_PAIR(1));
    mvwprintw(curWin, 0, cols / 2 - 7, " LeetCode TUI ");
    wattroff(curWin, COLOR_PAIR(1));

    wrefresh(curWin);
}