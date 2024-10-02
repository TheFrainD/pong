cmake_minimum_required(VERSION 3.22)
project(Pong LANGUAGES C CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
