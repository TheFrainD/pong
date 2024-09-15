add_executable(
  ${PROJECT_NAME}
  "src/game.cpp"
  "include/game.h"    
  "src/main.cpp"
)

if (WIN32)
  if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "/SUBSYSTEM:WINDOWS")
  endif()
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(
  ${PROJECT_NAME}
  ${PROJECT_NAME}-core
)

add_custom_target(
  copy_data COMMAND 
  ${CMAKE_COMMAND} -E copy_directory_if_different
  "${CMAKE_SOURCE_DIR}/data/"
  "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
)

add_dependencies(
  ${PROJECT_NAME} 
  copy_data
)

set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin")

install(
  TARGETS ${PROJECT_NAME}
  RUNTIME DESTINATION ${INSTALL_DIR}
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/data"
  DESTINATION ${INSTALL_DIR}
)