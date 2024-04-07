#include "textwindow.hpp"

#include <utility>

TextWindow::TextWindow(string content)
    : curWin(nullptr)
    , content(std::move(content))
    , contentLength(this->content.length())
    , totalLines(0) {}

WINDOW* TextWindow::drawWindow(int rows, int cols, int x, int y) {
    if (curWin != nullptr) return curWin;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // Color pair for yellow text on blue background

    curWin = newwin(rows, cols, x, y);
    scrollok(curWin, TRUE);
    refresh();

    for (auto &elem : content) {
        if (elem == '\n') ++totalLines;
    }

    wattron(curWin, COLOR_PAIR(3));

    string temp;
    for (int i = 0, lineBreak = 0, curLines = 0; i < contentLength && curLines < rows - 2; ++i) {
        if (content[i] == '\n' || i - lineBreak == cols) {
            ++curLines;
            if (i == contentLength - 1) {
                temp = content.substr(lineBreak, i - lineBreak);
                mvwprintw(curWin, curLines + 1, 2, "%s", temp.c_str());
                break;
            }
            temp = content.substr(lineBreak, i - lineBreak);
            mvwprintw(curWin, curLines + 1, 2, "%s", temp.c_str());

            while (content[i] == '\n' || content[i] == ' ') ++i;
            lineBreak = i;
        }
    }

    wattroff(curWin, COLOR_PAIR(3));

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Question ");
    wattroff(curWin, COLOR_PAIR(3));

    return curWin;
}

void TextWindow::refreshWindow(int rows, int cols, int x, int y) {}
