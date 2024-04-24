#include "textWindow.hpp"

TextWindow::TextWindow(const string &title, const string &content)
          : curWin(nullptr)
          , rows(0)
          , cols(0)
          , title(title)
          , content(content)
          , contentLength(0)
          , contentLines(0)
          , startLine(0) {}

WINDOW *TextWindow::drawWindow(int _rows, int _cols, int x, int y) {
    if (curWin != nullptr) return curWin;
    rows = _rows, cols = _cols;
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLACK, COLOR_CYAN); // Color pair for yellow text on blue background

    contentLength = static_cast<int>(content.length());

    for (int i = 0, startOfLine = 0; i < contentLength; ++i) {
        if (content[i] == '\n' || i - startOfLine == cols - 4) {
            ++contentLines;
            while (content[i] == '\n' || content[i] == ' ') ++i;
            startOfLine = i;
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
    mvwprintw(curWin, 0, 6, " %s ", title.c_str());
    wattroff(curWin, COLOR_PAIR(3));

    wrefresh(curWin);
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

    wrefresh(curWin);
}
