enable_testing()

find_package(GTest REQUIRED)

add_executable(
  ${PROJECT_NAME}-core-test
  "main.cpp"
  "collision_test.cpp"
)

target_link_libraries(
  ${PROJECT_NAME}-core-test
  gtest::gtest
  ${PROJECT_NAME}-core
)