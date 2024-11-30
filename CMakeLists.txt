cmake_minimum_required(VERSION 3.22)
project(leetcode_tui)

set(CMAKE_CXX_STANDARD 20)

find_package(cpr REQUIRED)
find_package(Curses REQUIRED)
find_package(CURL REQUIRED)
find_package(PostgreSQL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak")

add_executable(leetcode_tui src/main.cpp
        src/windows/menus/menuWindow.cpp
        src/windows/menus/menuWindow.hpp
        src/helpers/requests/dailyTaskRequest.hpp
        src/windows/textWindow.cpp
        src/windows/textWindow.hpp
        src/app.cpp
        src/app.hpp
        src/windows/menus/mainMenuWindow.cpp
        src/windows/menus/mainMenuWindow.hpp
        src/windows/menus/languageMenuWindow.cpp
        src/windows/menus/languageMenuWindow.hpp
        src/helpers/task.hpp
        src/helpers/task.cpp
        src/helpers/requests/dailyTaskRequest.cpp
        src/windows/menus/launchMenuWindow.cpp
        src/windows/menus/launchMenuWindow.hpp
        src/helpers/requests/runCodeRequests.cpp
        src/helpers/requests/runCodeRequests.hpp
        src/windows/searchBarWindow.cpp
        src/windows/searchBarWindow.hpp
        src/windows/menus/tasksMenuWindow.cpp
        src/windows/menus/tasksMenuWindow.hpp
        src/helpers/requests/questionListRequest.cpp
        src/helpers/requests/questionListRequest.hpp
        src/helpers/requests/singleTaskRequest.cpp
        src/helpers/requests/singleTaskRequest.hpp
        src/helpers/config.hpp
        src/helpers/constants.hpp
        src/helpers/constants.cpp
)

target_link_libraries(leetcode_tui PRIVATE cpr::cpr ncurses curl pqxx pq)