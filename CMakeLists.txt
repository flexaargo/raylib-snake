cmake_minimum_required(VERSION 4.0.0)

project(Snake CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED)

add_executable(game
    src/main.cpp
    src/menu.cpp
)

target_link_libraries(game PRIVATE raylib)
