#pragma once

#include "menuWindow.hpp"
#include "../searchBarWindow.hpp"

class SearchResultsMenu : public MenuWindow {
    void searchTasks(string &searchText);

public:
    SearchResultsMenu() = default;

    int handleKeyEvent() override;
};