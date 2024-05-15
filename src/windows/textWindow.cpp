#include "textWindow.hpp"

TextWindow::TextWindow(const string &title, const string &content)
        : curWin(nullptr)
        , rows(0)
        , cols(0)
        , x(0)
        , y(0)
        , title(title)
        , content(content)
        , contentLength(0)
        , contentLines(0)
        , startLine(0) {}

WINDOW *TextWindow::drawWindow(int _rows, int _cols, int _x, int _y) {
    if (curWin != nullptr) return curWin;
    scrollok(curWin, TRUE);
    rows = _rows, cols = _cols;
    x = _x, y = _y;

    contentLength = static_cast<int>(content.length());

    int limit = cols - 4;
    for (int i = 0; i < contentLength; ++i) {
        if (content[i] == '\n' || i - limit == 0) {
            ++contentLines;
            if (i - limit == 0) limit += cols - 4;
            while (content[i] == '\n' || content[i] == ' ') ++i;
        }
    }

    curWin = newwin(rows, cols, x, y);
    scrollok(curWin, TRUE);

    wattron(curWin, COLOR_PAIR(3));
    printWindowContent();
    wattroff(curWin, COLOR_PAIR(3));

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " %s ", title.c_str());
    wattroff(curWin, COLOR_PAIR(3));

    return curWin;
}

void TextWindow::refreshWindow(int _rows, int _cols, int _x, int _y) {
    scrollok(curWin, TRUE);
    startLine = 0;
    werase(curWin);

    if (_rows != rows || _cols != cols || _x != x || _y == y) {
        rows = _rows, cols = _cols;
        wresize(curWin, rows, cols);
        mvwin(curWin, x, y);
    }

    wattron(curWin, COLOR_PAIR(3));
    printWindowContent();
    wattroff(curWin, COLOR_PAIR(3));

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " %s ", title.c_str());
    wattroff(curWin, COLOR_PAIR(3));

    wnoutrefresh(curWin);
}

void TextWindow::handleKeyEvent() {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' :
                scrollUp();
                break;
            case 'j' :
                scrollDown();
                break;
            case 'c' :
                refreshWindow(20, 60, 10, 12);
                break;
            case 'r' :
                return;
            case 'q' :
                return;
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
    if (contentLines >= rows - 4 && contentLines - startLine >= rows - 4) {
        clearWindowContent();
        ++startLine;
        printWindowContent();
        wrefresh(curWin);
    }
}

void TextWindow::printWindowContent() const {
    int startOfLine = 0;
    int line = 0;
    int linesToPrint = 0;

    for (int i = 0; i <= contentLength && linesToPrint < rows - 4; ++i) {
        if (content[i] == '\n' || i - startOfLine == cols - 6 || contentLength - i == 0) {
            if (line >= startLine) {
                string temp = content.substr(startOfLine, i - startOfLine);
                mvwprintw(curWin, linesToPrint++ + 2, 3, "%s", temp.c_str());
            }
            ++line;
            while (content[i] == '\n' || content[i] == ' ') ++i;
            startOfLine = i;
        }
    }
}

void TextWindow::clearWindowContent() const {
    werase(curWin);

    wattron(curWin, COLOR_PAIR(4));
    box(curWin, ACS_VLINE, ACS_HLINE);
    wattroff(curWin, COLOR_PAIR(4));

    wattron(curWin, COLOR_PAIR(3));
    mvwprintw(curWin, 0, 6, " %s ", title.c_str());
    wattroff(curWin, COLOR_PAIR(3));
}