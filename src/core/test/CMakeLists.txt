find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(${PROJECT_NAME}-core-test "main.cpp" "collision_test.cpp"
                                         "scripts_test.cpp")

target_link_libraries(${PROJECT_NAME}-core-test gtest::gtest
                      ${PROJECT_NAME}-core)

gtest_discover_tests(${PROJECT_NAME}-core-test)

