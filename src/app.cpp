#include "app.hpp"

std::shared_ptr<pqxx::connection> App::conn = nullptr;

std::shared_ptr<Task> App::task = nullptr;

Task *App::taskPtr = nullptr;

bool App::isDatabaseExists(pqxx::connection &conn, const string &dbName) {
    pqxx::work tx{conn};
    const pqxx::result res = tx.exec(
        "SELECT 1 FROM pg_database WHERE datname = " + tx.quote(dbName) + " LIMIT 1;");

    return !res.empty();
}

bool App::isTableExists(pqxx::connection &conn, const string &tableName) {
    try {
        pqxx::work txn(conn);
        string query = "SELECT EXISTS (SELECT FROM information_schema.tables "
                       "WHERE table_schema = 'public' AND table_name = '" + tableName + "');";

        const pqxx::result result = txn.exec(query);

        bool tableExists = result[0][0].as<bool>();

        return tableExists;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

string App::readScript(const string &fileName) {
    std::ifstream script;

    if (RUNNING_IN_DOCKER)
        script.open(SCRIPTS_PATH_DOCKER + fileName);
    else
        script.open(SCRIPTS_PATH_LOCAL + fileName);

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
    use_default_colors();
    initColors();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(FALSE);

    getmaxyx(stdscr, totalRows, totalCols);

    try {
        if (bool configLoaded = Config::getConfig(); !configLoaded) {
            string title = "Warning";
            string content = "Please check the correctness of the config file";
            TextWindow warningWindow(title, content);
            WINDOW *warningWin = warningWindow.drawWindow(10, 80, TOTAL_ROWS / 2 - 5, TOTAL_COLS / 2 - 40, 4);
            wrefresh(warningWin);
            std::this_thread::sleep_for(std::chrono::seconds(5));
            endwin();
            return;
        }

        if (RUNNING_IN_DOCKER) {
            const char *db_url = std::getenv("DATABASE_URL");
            if (db_url == nullptr) {
                string title = "Warning";
                string content = "DATABASE_URL environment variable is not set";
                TextWindow warningWindow(title, content);
                WINDOW *warningWin = warningWindow.drawWindow(10, 80, TOTAL_ROWS / 2 - 5, TOTAL_COLS / 2 - 40, 4);
                wrefresh(warningWin);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                endwin();
                return;
            }

            conn = std::make_shared<pqxx::connection>(db_url);

            if (bool tableExists = isTableExists(*conn, "tasks"); !tableExists) {
                string createTableScript = readScript("create_table.sql");
                pqxx::nontransaction tx{*conn};
                tx.exec(createTableScript);
                tx.commit();
            }
        } else {
            const string configOptions = "dbname=" + Config::getDbname() + " user=" + Config::getUser()
                                         + " password=" + Config::getPassword() + " hostaddr=" + Config::getHostaddr() +
                                         " port=" +
                                         Config::getPort();
            const string newOptions = "dbname=leetcode_tui user=" + Config::getUser()
                                      + " password=" + Config::getPassword() + " host=" + Config::getHostaddr() +
                                      " port=" +
                                      Config::getPort();

            conn = std::make_shared<pqxx::connection>(configOptions);
            bool databaseExists = isDatabaseExists(*conn, "leetcode_tui");

            if (!databaseExists) {
                string createDbScript = readScript("create_db.sql");
                pqxx::nontransaction tx{*conn};
                tx.exec(createDbScript);
                tx.commit();
            }

            conn->close();

            conn = std::make_shared<pqxx::connection>(newOptions);

            if (!databaseExists) {
                string createTableScript = readScript("create_table.sql");
                pqxx::nontransaction tx{*conn};
                tx.exec(createTableScript);
                tx.commit();
            }
        }

        task = std::make_shared<Task>(*conn);
        taskPtr = &*task;

        string statsStr = "STATS";
        string tempStr = " Here will be user stats";
        TextWindow userStatsWindow(statsStr, tempStr);
        WINDOW *userStatsWin = userStatsWindow.drawWindow(TOTAL_ROWS / 2 + 5, TOTAL_COLS / 2 - 5, 5, TOTAL_ROWS / 4, 0);

        MainMenuWindow mainMenuWindow(stdscr, {
                                          "Here will be the task   ",
                                          "Task lists              ",
                                          "Search                  ",
                                      });
        WINDOW *mainMenuWin = mainMenuWindow.drawWindow(TOTAL_ROWS / 2 + 5, TOTAL_COLS / 2 - 5,
                                                        5 + TOTAL_COLS / 2 - 5 + 1,
                                                        TOTAL_ROWS / 4,
                                                        4, 5);

        wrefresh(stdscr);
        wrefresh(userStatsWin);
        wrefresh(mainMenuWin);

        while (true) {
            menuCodes curCode = mainMenuWindow.handleKeyEvent(taskPtr);
            if (curCode == menuCodes::quit) break;
            if (curCode == menuCodes::refreshWin) {
                userStatsWindow.refreshWindow(TOTAL_ROWS / 2 + 5, TOTAL_COLS / 2 - 5, 5, TOTAL_ROWS / 4, 0);
                mainMenuWindow.refreshWindow(TOTAL_ROWS / 2 + 5, TOTAL_COLS / 2 - 5,
                                             5 + TOTAL_COLS/ 2 - 5 + 1,
                                             TOTAL_ROWS / 4,
                                             4, 5);
                wattron(stdscr, COLOR_PAIR(0));
                if (mainMenuWindow.getCurItem() == 1) {
                    mvwprintw(stdscr, TOTAL_ROWS - 2, 3,
                              "Press 'r' to read the task | 'o' to open it in nvim | 'c' to refresh code snippet");
                    if (!mainMenuWindow.getRefreshCodeSnippetStatus())
                        mvwprintw(stdscr, TOTAL_ROWS / 2 - 1, TOTAL_COLS / 2 + 16,
                                  "'Enter' to Run or Submit");
                }
                wattroff(stdscr, COLOR_PAIR(0));
                wnoutrefresh(stdscr);
                wnoutrefresh(mainMenuWin);
                wnoutrefresh(userStatsWin);
                doupdate();
            }
        }

        endwin();
    } catch (std::exception const &e) {
        std::cerr << "ERROR: " << e.what() << '\n';
    }
}
