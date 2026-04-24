cmake_minimum_required(VERSION 4.1)
project(Chess_Game)

include_directories(vendor/sqlite src)

add_executable(${PROJECT_NAME}
        src/main.cpp
        vendor/sqlite/sqlite3.c
)