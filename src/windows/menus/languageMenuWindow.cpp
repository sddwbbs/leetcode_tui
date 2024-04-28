#include "languageMenuWindow.hpp"

LanguageMenuWindow::LanguageMenuWindow(WINDOW *parentWin)
        : MenuWindow(parentWin, lang) {}

int LanguageMenuWindow::handleKeyEvent(Task *task) {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(2, 5);
                return static_cast<int>(menuCodes::ok);
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(2, 5);
                return static_cast<int>(menuCodes::ok);
            }

            case 10 : {
            }

            case 'q' : {
                return static_cast<int>(menuCodes::quit);
            }

            default:
                return static_cast<int>(menuCodes::ok);
        }
    }

    return static_cast<int>(menuCodes::ok);
}
