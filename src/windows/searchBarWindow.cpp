#include "searchBarWindow.hpp"

WINDOW *SearchBarWindow::drawWindow(int _rows, int _cols, int _x, int _y) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols, x = _x, y = _y;

    curWin = newwin(rows, cols, y, x);
    refresh();
    // scrollok(curWin, TRUE);

    wattron(curWin, COLOR_PAIR(0));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(0));

    wattron(curWin, COLOR_PAIR(0));
    mvwprintw(curWin, 0, 6, " %s ", " Search \U0001F50E ");
    wattroff(curWin, COLOR_PAIR(0));

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

            case 127 :

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
        wattron(curWin, COLOR_PAIR(0));
        box(curWin, ACS_VLINE, ACS_HLINE);
        wattroff(curWin, COLOR_PAIR(0));

        wattron(curWin, COLOR_PAIR(0));
        mvwprintw(curWin, 0, 6, " %s ", " Search \U0001F50E ");
        wattroff(curWin, COLOR_PAIR(0));

        mvwprintw(curWin, 1, 2, "%s", searchText.c_str());
        wrefresh(curWin);
    }
    return searchBarCodes::quit;
}

string SearchBarWindow::getSearchText() const { return searchText; }
