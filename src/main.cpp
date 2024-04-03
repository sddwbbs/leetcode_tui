#include "mainwindow.hpp"
#include "menuwindow.hpp"
#include "textwindow.hpp"
#include "helpers/dailyquestionrequest.hpp"

#include <ncurses.h>
#include <cstdlib>

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
                    TextWindow questionTextWindow(DailyQuestionRequest::getQuestion());
                    WINDOW* questionTextWin = questionTextWindow.drawWindow(30, 100, row / 2 - 15, col / 2 - 50);
                    wrefresh(questionTextWin);

                    int ch2;
                    while ((ch2 = getch()) != 27) { // Continue loop until ESC key is pressed
                        questionTextWindow.handleKeyEvent(ch2);
                    }
                    questionTextWindow.closeWindow();

                    MainWindow::refreshWindow(row, col, 0, 0);
                    mainMenuWindow.refreshWindow(10, 30, row / 2 - 5, col / 2 - 15);

                    wrefresh(mainWin);
                    wrefresh(mainMenuWin);
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
            mainMenuWindow.refreshWindow(10, 30, row / 2 - 5, col / 2 - 15);

            wrefresh(mainWin);
            wrefresh(mainMenuWin);
        }
    }

    endwin();
}
