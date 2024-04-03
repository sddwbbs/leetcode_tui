#include "textwindow.hpp"

#include <utility>

TextWindow::TextWindow(string content) : curWin(nullptr), content(std::move(content)), scrollPosition(0) {}

WINDOW* TextWindow::drawWindow(int row, int col, int x, int y) {
    if (curWin != nullptr) return curWin;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // Color pair for yellow text on blue background

    curWin = newwin(row, col, x, y);
    scrollok(curWin, TRUE);
    keypad(curWin, TRUE);
    refresh();

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Question ");
    wattroff(curWin, COLOR_PAIR(3));

    refreshText();

    return curWin;
}

void TextWindow::refreshText() {
    wclear(curWin);
    box(curWin, ACS_VLINE, ACS_HLINE);
    mvwprintw(curWin, 0, 6, " Question ");

    int maxY, maxX;
    getmaxyx(curWin, maxY, maxX);

    int lineNum = 2;
    int contentStart = scrollPosition;
    int contentEnd = scrollPosition + maxY - 4; // Leave space for borders and title

    size_t i = contentStart;
    while (i < content.length() && lineNum < maxY - 2) {
        if (content[i] == '\n') {
            lineNum++;
        } else {
            mvwaddch(curWin, lineNum, 2, content[i]);
            lineNum++;
        }
        i++;
    }

    wrefresh(curWin);
}

void TextWindow::scrollUp() {
    if (scrollPosition > 0) {
        scrollPosition--;
        refreshText();
    }
}

void TextWindow::scrollDown() {
    if (scrollPosition < content.length() - 1) {
        scrollPosition++;
        refreshText();
    }
}

void TextWindow::handleKeyEvent(int key) {
    switch (key) {
        case 'j':
            scrollDown();
            break;
        case 'k':
            scrollUp();
            break;
        case 27: // ESC key
            closeWindow();
            break;
    }
}

void TextWindow::closeWindow() {
    if (curWin != nullptr) {
        delwin(curWin);
        curWin = nullptr;
    }
}

void TextWindow::refreshWindow(int row, int col, int x, int y) {}