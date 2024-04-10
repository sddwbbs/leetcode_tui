#include "textWindow.hpp"

#include <utility>

TextWindow::TextWindow(string content)
    : curWin(nullptr)
    , rows(0)
    , cols(0)
    , content(std::move(content))
    , contentLength(0)
    , contentLines(0)
    , startLine(0) {}

WINDOW* TextWindow::drawWindow(int _rows, int _cols, int x, int y) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // Color pair for yellow text on blue background

    for (auto &elem : content) {
        if (elem == '\n') ++contentLines;
    }

    contentLength = static_cast<int>(content.length());

    curWin = newwin(rows, cols, x, y);
    scrollok(curWin, TRUE);

    wattron(curWin, COLOR_PAIR(3));
    printWindowContent();
    wattroff(curWin, COLOR_PAIR(3));

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Question ");
    wattroff(curWin, COLOR_PAIR(3));

    return curWin;
}

void TextWindow::refreshWindow(int _rows, int _cols, int x, int y) {
    startLine = 0;
    werase(curWin);
    wrefresh(curWin);
    rows = _rows, cols = _cols;
    wresize(curWin, rows, cols);
    mvwin(curWin, x, y);

    wattron(curWin, COLOR_PAIR(3));
    printWindowContent();
    wattroff(curWin, COLOR_PAIR(3));

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Question ");
    wattroff(curWin, COLOR_PAIR(3));

    wrefresh(curWin);
}

void TextWindow::handleKeyEvent() {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : scrollUp(); break;
            case 'j' : scrollDown(); break;
            case 'c' : refreshWindow(20, 60, 10, 12); break;
            case 'r' : return;
            case 'q' : return;
        }
    }
}

void TextWindow::scrollUp() {
    if (startLine > 0) {
        clearWindowContent();
        --startLine;
        printWindowContent();
        wrefresh(curWin);
    }
}

void TextWindow::scrollDown() {
    if (startLine != contentLines && contentLines - startLine > rows) {
        clearWindowContent();
        ++startLine;
        printWindowContent();
        wrefresh(curWin);
    }
}

void TextWindow::printWindowContent() {
    int lineBreak = 0;
    int curLine = 0;
    int linesToPrint = 1;
    for (int i = 0; i < contentLength && linesToPrint < rows - 2; ++i) {
        if (content[i] == '\n' || i - lineBreak == cols - 5) {
            if (i == contentLength - 1) {
                if (curLine >= startLine) {
                    mvwprintw(curWin, linesToPrint + 1, 2, " %s",
                              content.substr(lineBreak, i - lineBreak).c_str());
                    ++linesToPrint;
                }
                break;
            }

            if (curLine >= startLine) {
                mvwprintw(curWin, linesToPrint + 1, 2, " %s",
                          content.substr(lineBreak, i - lineBreak).c_str());
                ++linesToPrint;
            }

            while (content[i] == '\n' || content[i] == ' ') ++i;
            lineBreak = i;
            ++curLine;
        }
    }
}

void TextWindow::clearWindowContent() {
    werase(curWin);

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " Question ");
    wattroff(curWin, COLOR_PAIR(3));

    wrefresh(curWin);
}
