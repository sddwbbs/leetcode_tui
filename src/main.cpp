#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <curl/curl.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <regex>
#include "mainwindow.hpp"
#include "menuwindow.hpp"

using json = nlohmann::json;

size_t writeCallback(void* contents, size_t size, size_t nmemb, std::stringstream* ss) {
    size_t totalSize = size * nmemb;
    ss->write(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string removeHtmlTags(const std::string &htmlStr) {
    std::regex regex("<[^>]*>");
    return std::regex_replace(htmlStr, regex, "");
}

int main(int argc, char **argv)
{
    initscr();
    start_color();

    int row, col;
    getmaxyx(stdscr, row, col);

    WINDOW* mainWin = MainWindow::drawWindow(row, col, 0, 0);
    MenuWindow mainMenuWindow({"Open Nvim        ", "Open Daily Task  ", "Open Task List   "});
    WINDOW* mainMenuWin = mainMenuWindow.drawWindow(10, 30, row / 2 - 5, col / 2 - 15);

    wrefresh(mainWin);
    wrefresh(mainMenuWin);

    noecho();
    curs_set(0);

    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case 'k' :
                if (mainMenuWindow.curItem > 0)
                    mainMenuWindow.menuUp();
                break;
            case 'j' :
                if (mainMenuWindow.curItem < mainMenuWindow.menuSize - 1)
                    mainMenuWindow.menuDown();
                break;
            case 't':
            {
                std::string centerText = "Hello, Center Text!";

                int centerY = getmaxy(mainWin) / 2;
                int centerX = (getmaxx(mainWin) - centerText.length()) / 2;

                mvwprintw(mainWin, centerY, centerX, "%s", centerText.c_str());
                wrefresh(mainWin);
            }
                break;
            case 10 :
                if (mainMenuWindow.curItem == 0) {
                    endwin();
                    system("nvim");
                    initscr();
                    refresh();
                }

                if (mainMenuWindow.curItem == 1) {
                    std::string htmlTask;
                    CURL *curl;
                    CURLcode res;
                    std::stringstream responseStream;
                    json jsonResponse;

                    curl_global_init(CURL_GLOBAL_DEFAULT);
                    curl = curl_easy_init();
                    if (curl) {
                        curl_easy_setopt(curl, CURLOPT_URL, "https://alfa-leetcode-api.onrender.com/daily");
                        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
                        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStream);

                        res = curl_easy_perform(curl);
                        if (res != CURLE_OK) {
                            std::cerr << "Failed to perform HTTP request: " << curl_easy_strerror(res) << std::endl;
                        } else {
                            // Parse the response as JSON
                            try {
                                jsonResponse = json::parse(responseStream.str());
                            } catch (const std::exception& e) {
                                std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
                                return 1;
                            }
                        }
                        curl_easy_cleanup(curl);
                    } else {
                        std::cerr << "Failed to initialize libcurl." << std::endl;
                    }

                    curl_global_cleanup();
                    std::string task = removeHtmlTags(jsonResponse["question"]);
                    mvwprintw(mainWin, 5, 4, task.c_str());
                    wrefresh(mainWin);
                }
                break;
        }
        wrefresh(mainWin);
        wrefresh(mainMenuWin);

        int new_row, new_col;
        getmaxyx(stdscr, new_row, new_col);
        if (new_row != row || new_col != col || ch == 'c') {
            row = new_row, col = new_col;
            MainWindow::refreshWindow(row, col, 0, 0);
            mainMenuWindow.refreshWindow(10, 30, row / 2 - 10, col / 2 - 30);

            wrefresh(mainWin);
            wrefresh(mainMenuWin);
        }
    }

    endwin();
}
