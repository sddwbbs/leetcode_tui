#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <curl/curl.h>
#include <sstream>
#include <nlohmann/json.hpp>
#include <string>
#include <regex>

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

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    int row, col;
    getmaxyx(stdscr, row, col);

    WINDOW *win = newwin(row, col, 0, 0);
    WINDOW *menu_win = newwin(10, 30, row / 2 - 5, col / 2 - 15);
    refresh();

    wattron(win, COLOR_PAIR(2));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(2));

    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 0, col / 2 - 7, " LeetCode TUI ");
    wattroff(win, COLOR_PAIR(1));

    const char* menu[] = {"Open Nvim", "Open Daily Task", "Open Task List"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    int current_item = 0;

    wattron(menu_win, COLOR_PAIR(2));
    box(menu_win, 0, 0);
    wattroff(menu_win, COLOR_PAIR(2));

    wattron(menu_win, COLOR_PAIR(1));
    mvwprintw(menu_win, 0, col / 2, "");
    wattroff(menu_win, COLOR_PAIR(1));

    for (int i = 0; i < menu_size; ++i) {
        mvwprintw(menu_win, i + 2, 5, menu[i]);
    }

    wattron(menu_win, A_REVERSE);
    mvwprintw(menu_win, current_item + 2, 5, menu[current_item]);
    wattroff(menu_win, A_REVERSE);

    wrefresh(win);
    wrefresh(menu_win);

    noecho();
    curs_set(0);

    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case 'k' :
                if (current_item > 0) {
                    mvwprintw(menu_win, current_item + 2, 5, menu[current_item]);
                    --current_item;
                    wattron(menu_win, A_REVERSE);
                    mvwprintw(menu_win, current_item + 2, 5, menu[current_item]);
                    wattroff(menu_win, A_REVERSE);
                }
                break;
            case 'j' :
                if (current_item < menu_size - 1) {
                    mvwprintw(menu_win, current_item + 2, 5, menu[current_item]);
                    ++current_item;
                    wattron(menu_win, A_REVERSE);
                    mvwprintw(menu_win, current_item + 2, 5, menu[current_item]);
                    wattroff(menu_win, A_REVERSE);
                }
                break;
            case 't':
            {
                std::string centerText = "Hello, Center Text!";

                int centerY = getmaxy(win) / 2;
                int centerX = (getmaxx(win) - centerText.length()) / 2;

                mvwprintw(win, centerY, centerX, centerText.c_str());
                wrefresh(win);
            }
                break;
            case 10 :
                if (current_item == 0) {
                    endwin();
                    system("nvim");
                    initscr();
                    refresh();
                }

                if (current_item == 1) {
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
                    mvwprintw(win, 5, 4, task.c_str());
                    wrefresh(win);
                }
                break;
        }
        wrefresh(win);
        wrefresh(menu_win);

        int new_row, new_col;
        getmaxyx(stdscr, new_row, new_col);
        if (new_row != row || new_col != col || ch == 'c') {
            row = new_row;
            col = new_col;
            wresize(win, row, col);
            mvwin(win, 0, 1);
            werase(win);
            wattron(win, COLOR_PAIR(2));
            box(win, 0, 0);
            wattroff(win, COLOR_PAIR(2));
            wattron(win, COLOR_PAIR(1));
            mvwprintw(win, 0, col / 2 - 7, " LeetCode TUI ");
            wattroff(win, COLOR_PAIR(1));

            wattron(menu_win, COLOR_PAIR(2));
            box(menu_win, 0, 0);
            wattroff(menu_win, COLOR_PAIR(2));

            wattron(menu_win, COLOR_PAIR(1));
            mvwprintw(menu_win, 0, col / 2, "");
            wattroff(menu_win, COLOR_PAIR(1));

            for (int i = 0; i < menu_size; i++) {
                mvwprintw(menu_win, i + 2, 5, menu[i]);
            }

            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, current_item + 2, 5, menu[current_item]);
            wattroff(menu_win, A_REVERSE);
            wrefresh(win);
            wrefresh(menu_win);
        }
    }

    endwin();
}
