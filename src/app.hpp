#pragma once

#include "mainWindow.hpp"
#include "menuWindow.hpp"
#include "textWindow.hpp"
#include "helpers/task.hpp"

#include <ncurses.h>
#include <cstdlib>

const vector<string> languages = {"C++        ",
                                  "Java       ",
                                  "Python     ",
                                  "Python3    ",
                                  "C          ",
                                  "C#         ",
                                  "JavaScript ",
                                  "TypeScript ",
                                  "PHP        ",
                                  "Swift      ",
                                  "Kotlin     ",
                                  "Dart       ",
                                  "Go         ",
                                  "Ruby       ",
                                  "Scala      ",
                                  "Rust       ",
                                  "Racket     ",
                                  "Erlang     ",
                                  "Elixir     "};

class App {
public:
    static void startApp();
};