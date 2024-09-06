cmake_minimum_required(VERSION 3.22)
project(pong LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ${PROJECT_NAME}_SOURCES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

find_package(raylib REQUIRED)

target_link_libraries(${PROJECT_NAME} raylib)