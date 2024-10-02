find_package(GTest REQUIRED)

include(GoogleTest)

set(TARGET "${CORE_LIB}-test")

file(GLOB_RECURSE "${TARGET}_SOURCES" "*.cpp" "*.h")

add_executable(${TARGET} ${${TARGET}_SOURCES})

target_link_libraries(${TARGET} gtest::gtest ${CORE_LIB})

gtest_discover_tests(${TARGET})
