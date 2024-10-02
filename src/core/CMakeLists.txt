find_package(raylib REQUIRED)
find_package(EnTT REQUIRED)
find_package(sol2 REQUIRED)
find_package(spdlog REQUIRED)

set(TARGET ${CORE_LIB})

file(GLOB_RECURSE "${TARGET}_SOURCES" "src/*.cpp" "include/*.h")

message(STATUS "Configuring: ${TARGET}")

add_library(${TARGET} STATIC ${${TARGET}_SOURCES})

target_include_directories(${TARGET}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${TARGET} PUBLIC raylib EnTT::EnTT sol2::sol2
                                       spdlog::spdlog)

add_subdirectory(test)
