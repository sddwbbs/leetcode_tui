cmake_minimum_required(VERSION 3.27)
project(leetcode_tui)

set(CMAKE_CXX_STANDARD 20)

find_package(cpr REQUIRED)
find_package(Curses REQUIRED)
find_package(CURL REQUIRED)
find_package(PostgreSQL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak")

add_executable(leetcode_tui src/main.cpp
        src/mainWindow.hpp
        src/mainWindow.cpp
        src/menuWindow.cpp
        src/menuWindow.hpp
        src/helpers/requests/dailyTaskRequest.hpp
        src/textWindow.cpp
        src/textWindow.hpp
        src/app.cpp
        src/app.hpp
        src/mainMenuWindow.cpp
        src/mainMenuWindow.hpp
        src/languageMenuWindow.cpp
        src/languageMenuWindow.hpp
        src/helpers/task.hpp
        src/helpers/task.cpp
        src/helpers/requests/codeSnippetRequest.hpp
        src/helpers/requests/dailyTaskRequest.cpp
)

target_link_libraries(leetcode_tui PRIVATE cpr::cpr ncurses curl pqxx pq gumbo)
