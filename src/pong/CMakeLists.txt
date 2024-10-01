set(TARGET ${PROJECT_NAME})

file(GLOB_RECURSE "${TARGET}_SOURCES" "src/*.cpp" "include/*.h")

add_executable(${TARGET} ${${TARGET}_SOURCES})

if(WIN32)
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(${TARGET} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
  endif()
endif()

target_include_directories(${TARGET}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${TARGET} ${CORE_LIB})

add_custom_target(
  copy_data
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
          "${CMAKE_SOURCE_DIR}/data/" "$<TARGET_FILE_DIR:${TARGET}>/data")

add_dependencies(${TARGET} copy_data)

set(INSTALL_DIR "${CMAKE_SOURCE_DIR}/bin")

install(TARGETS ${TARGET} RUNTIME DESTINATION ${INSTALL_DIR})

install(DIRECTORY "${CMAKE_SOURCE_DIR}/data" DESTINATION ${INSTALL_DIR})

