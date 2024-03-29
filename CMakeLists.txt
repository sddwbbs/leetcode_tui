cmake_minimum_required(VERSION 3.27)
project(leetcode_tui)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
find_package(CURL REQUIRED)

add_executable(leetcode_tui main.cpp)
target_link_libraries(leetcode_tui ncurses curl)
