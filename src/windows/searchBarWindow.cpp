#include "searchBarWindow.hpp"

WINDOW *SearchBarWindow::drawWindow(int _rows, int _cols, int _x, int _y) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols, x = _x, y = _y;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // Color pair for yellow text on blue background

    curWin = newwin(rows, cols, x, y);
    scrollok(curWin, TRUE);

    wattron(curWin, COLOR_PAIR(3));
    wattroff(curWin, COLOR_PAIR(3));

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " %s %s ", "\U0001F50D", " Search ");
    wattroff(curWin, COLOR_PAIR(3));

    return curWin;
}

void SearchBarWindow::refreshWindow(int _rows, int _cols, int _x, int _y) {

}

searchBarCodes SearchBarWindow::handleKeyEvent() {
    char ch;

    while ((ch = getch()) != 27) {
        switch (ch) {
            case '\n' : {
                if (prevSearchText == searchText)
                    return searchBarCodes::ok;
                prevSearchText = searchText;
                return searchBarCodes::textTyped;
            }

            case 127 : {
                if (!searchText.empty()) searchText.pop_back();
            }
                break;

            case 7 : {
                if (!searchText.empty()) searchText.pop_back();
            }
                break;

            case 23 : {
                size_t lastSpaceIndex = searchText.find_last_of(' ');
                if (lastSpaceIndex != string::npos) {
                    searchText = searchText.substr(0, lastSpaceIndex);
                } else if (!searchText.empty()) {
                    searchText.clear();
                }
            }
                break;

            case 'q' : {
                return searchBarCodes::quit;
            }

            default:
                if (searchText.length() < cols - 4) searchText.push_back(ch);
        }

        werase(curWin);
        wattron(curWin, COLOR_PAIR(4));
        box(curWin, ACS_VLINE, ACS_HLINE);
        wattroff(curWin, COLOR_PAIR(4));

        wattron(curWin, COLOR_PAIR(3));
        mvwprintw(curWin, 0, 6, " %s %s ", "\U0001F50D", " Search ");
        wattroff(curWin, COLOR_PAIR(3));

        mvwprintw(curWin, 1, 2, "%s", searchText.c_str());
        wrefresh(curWin);
    }
    return searchBarCodes::quit;
}

string SearchBarWindow::getSearchText() const { return searchText; }
