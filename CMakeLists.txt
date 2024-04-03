cmake_minimum_required(VERSION 3.27)
project(leetcode_tui)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
find_package(CURL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak")

add_executable(leetcode_tui src/main.cpp
        src/mainwindow.hpp
        src/mainwindow.cpp
        src/menuwindow.cpp
        src/menuwindow.hpp
        src/request.hpp
        src/helpers/dailyquestionrequest.hpp
        src/textwindow.cpp
        src/textwindow.hpp
        src/helpers/questionrequest.hpp
        src/helpers/dailyquestionrequest.cpp
        src/helpers/questionrequest.cpp)
target_link_libraries(leetcode_tui ncurses curl)
