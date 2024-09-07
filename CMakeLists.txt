cmake_minimum_required(VERSION 3.22)
project(pong LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ${PROJECT_NAME}_SOURCES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME}
        src/main.cpp
        include/game.h
        src/game.cpp
        include/comp/player.h
        include/comp/sprite_renderer.h
        include/sys/render.h
        src/sys/render.cpp
        include/comp/transform.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(raylib REQUIRED)
find_package(EnTT REQUIRED)

target_link_libraries(${PROJECT_NAME} raylib EnTT::EnTT)