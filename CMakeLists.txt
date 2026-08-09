cmake_minimum_required(VERSION 4.0.0)

project(Snake CXX)

set(CMAKE CXX_STANDARD 20)
set(CMAKE CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(game
    src/main.cpp
)

target_link_libraries(game PRIVATE raylib)
