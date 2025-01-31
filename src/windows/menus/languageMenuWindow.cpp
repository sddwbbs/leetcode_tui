#include "languageMenuWindow.hpp"

LanguageMenuWindow::LanguageMenuWindow(WINDOW *parentWin, const vector<string> &menuItems)
        : MenuWindow(parentWin, menuItems, "SELECT LANGUAGE") {}

menuCodes LanguageMenuWindow::handleKeyEvent() {
    int ch;
    while ((ch = getch()) != 27) {
        switch (ch) {
            case 'k' : {
                menuUp(rowsPadding, colsPadding);
                return menuCodes::refreshWin;
            }

            case 'j' : {
                menuDown(rowsPadding, colsPadding);
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
