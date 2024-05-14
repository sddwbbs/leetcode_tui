#include "languageMenuWindow.hpp"

LanguageMenuWindow::LanguageMenuWindow(WINDOW *parentWin)
        : MenuWindow(parentWin, lang) {}

menuCodes LanguageMenuWindow::handleKeyEvent() {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                if (curItem > 0) menuUp(2, 5);
                return menuCodes::refreshWin;
            }

            case 'j' : {
                if (curItem < menuSize - 1) menuDown(2, 5);
                return menuCodes::refreshWin;
            }

            case 10 : {
                return menuCodes::itemSelected;
            }

            case 'q' :

            case 'o' : {
                return menuCodes::quit;
            }
        }
    }

    return menuCodes::ok;
}
