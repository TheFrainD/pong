enable_testing()

find_package(GTest REQUIRED)

add_executable(
  ${PROJECT_NAME}-test
  main.cpp
  test.cpp
)

target_link_libraries(
  ${PROJECT_NAME}-test
  gtest::gtest
)