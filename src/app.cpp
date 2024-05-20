#include "app.hpp"

void App::initColors() {
    init_color(COLOR_BLACK, 82, 35, 72);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN);
    init_pair(6, COLOR_BLACK, COLOR_GREEN);
    init_pair(7, COLOR_YELLOW, COLOR_BLACK);
    init_pair(8, COLOR_RED, COLOR_BLACK);
}

void App::startApp() {
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    initColors();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    try {
        pqxx::connection conn("dbname=leetcode_tui user=postgres password=8080 hostaddr=127.0.0.1 port=5432");

        WINDOW *mainWin = MainWindow::drawWindow(rows, cols, 0, 0);

        try {
            Config::setConfig(std::getenv("csrftoken"), std::getenv("LEETCODE_SESSION"));
        } catch(std::exception const &e) {
            string title = "Warning";
            string content = "Please create environment variables with your csrf token and leetcode session";
            TextWindow warningWindow(title, content);
            WINDOW* warningWin = warningWindow.drawWindow(10, 80, rows / 2 - 5, cols / 2 - 40, 4);
            wrefresh(warningWin);
            getch();
            endwin();
            return;
        }

        MainMenuWindow mainMenuWindow(mainWin, {"Open Nvim        ", "Open Daily Task  ", "Open Tasks List  ", "\U0001F50D Search        "});
        WINDOW *mainMenuWin = mainMenuWindow.drawWindow(8, 30, rows / 2 - 4, cols / 2 - 15, 2, 5);

        wrefresh(mainWin);
        wrefresh(mainMenuWin);

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
                doupdate();
            }
        }

        delete task;
        endwin();

    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return;
    }
}