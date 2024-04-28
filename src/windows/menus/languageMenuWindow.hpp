#pragma once

#include "menuWindow.hpp"

#include <unordered_map>

using std::unordered_map;

const vector<string> lang = {
        "C++ ",
        "Java ",
        "Python ",
        "Python3 ",
        "C ",
        "C# ",
        "JavaScript ",
        "TypeScript ",
        "PHP ",
        "Swift ",
        "Kotlin ",
        "Dart ",
        "Go ",
        "Ruby ",
        "Scala ",
        "Rust ",
        "Racket ",
        "Erlang ",
        "Elixir "
};

class LanguageMenuWindow : public MenuWindow {
public:
    const unordered_map<string, string> langExt = {
            { "C++", "cpp" },
            { "Java", "java" },
            { "Python", "py" },
            { "Python3", "py" },
            { "C", "c" },
            { "C#", "cs" },
            { "JavaScript", "js" },
            { "TypeScript", "ts" },
            { "PHP", "php" },
            { "Swift", "swift" },
            { "Kotlin", "kt" },
            { "Dart", "dart" },
            { "Go", "go" },
            { "Ruby", "rb" },
            { "Scala", "scala" },
            { "Rust", "rs" },
            { "Racket", "rkt" },
            { "Erlang", "erl" },
            { "Elixir", "ex" }
    };

    explicit LanguageMenuWindow(WINDOW *parentWin);

    int handleKeyEvent(Task *task) override;
};