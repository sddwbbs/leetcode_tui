#include "app.hpp"

bool App::isDatabaseExists(pqxx::connection &conn, const string &dbName) {
    pqxx::work tx{conn};
    const pqxx::result res = tx.exec(
        "SELECT 1 FROM pg_database WHERE datname = " + tx.quote(dbName) + " LIMIT 1;");

    return !res.empty();
}

string App::readScript(const string &fileName) {
    std::ifstream script("../src/helpers/scripts/" + fileName);
    std::stringstream buffer;
    buffer << script.rdbuf();
    return buffer.str();
}

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
    setlocale(LC_ALL, "en_US.UTF-8");
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
        try {
            Config::getConfig();
        } catch (std::exception const &e) {
            string title = "Warning";
            string content = "Please check the correctness of the config.conf file";
            TextWindow warningWindow(title, content);
            WINDOW *warningWin = warningWindow.drawWindow(10, 80, rows / 2 - 5, cols / 2 - 40, 4);
            wrefresh(warningWin);
            getch();
            endwin();
            return;
        }

        const string configOptions = "dbname=" + Config::getDbname() + " user=" + Config::getUser()
                         + " password=" + Config::getPassword() + " hostaddr=" + Config::getHostaddr() + " port=" +
                         Config::getPort();
        const string newOptions = "dbname=leetcode_tui user=" + Config::getUser()
             + " password=" + Config::getPassword() + " hostaddr=" + Config::getHostaddr() + " port=" +
             Config::getPort();

        pqxx::connection conn(configOptions);
        bool databaseExists = isDatabaseExists(conn, "leetcode_tui");

        if (!databaseExists) {
            string createDbScript = readScript("create_db.sql");
            pqxx::nontransaction tx{conn};
            tx.exec(createDbScript);
            tx.commit();
        }

        conn.close();

        pqxx::connection newConn(newOptions);

        if (!databaseExists) {
            string createTableScript = readScript("create_table.sql");
            pqxx::nontransaction tx{newConn};
            tx.exec(createTableScript);
            tx.commit();
        }

        WINDOW *mainWin = MainWindow::drawWindow(rows, cols, 0, 0);
        MainMenuWindow mainMenuWindow(mainWin, {
                                          "Open Nvim        ", "Open Daily Task  ", "Open Tasks List  ",
                                          "Search           "
                                      });
        WINDOW *mainMenuWin = mainMenuWindow.drawWindow(8, 30, rows / 2 - 4, cols / 2 - 15, 2, 5);

        wrefresh(mainWin);
        wrefresh(mainMenuWin);

        Task *task = new Task(newConn);

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