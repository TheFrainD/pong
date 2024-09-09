cmake_minimum_required(VERSION 3.22)
project(pong LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ${PROJECT_NAME}_SOURCES src/*.cpp src/*.h)
add_executable(${PROJECT_NAME}
        src/main.cpp
        include/game.h
        src/game.cpp
        include/comp/sprite.h
        include/sys/render.h
        src/sys/render.cpp
        include/comp/transform.h
        include/sys/scripts.h
        src/sys/scripts.cpp
        include/comp/script.h
        src/comp/script.cpp
        include/comp/name.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(raylib REQUIRED)
find_package(EnTT REQUIRED)
find_package(sol2 REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME}
        raylib
        EnTT::EnTT
        sol2::sol2
        spdlog::spdlog
)

add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/data/"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
        COMMENT "Copying data"
)
add_dependencies(${PROJECT_NAME} copy_data)