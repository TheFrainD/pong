find_package(raylib REQUIRED)
find_package(EnTT REQUIRED)
find_package(sol2 REQUIRED)
find_package(spdlog REQUIRED)

set(CORE_LIB ${PROJECT_NAME}-core)

add_library(
  ${CORE_LIB} STATIC
  "src/comp/script/builder.cpp"
  "src/sys/physics.cpp"
  "src/sys/render.cpp"
  "src/sys/scripts.cpp"
  "src/entity/builder.cpp"
  "include/core/comp/collider.h"
  "include/core/comp/label.h"
  "include/core/comp/name.h"
  "include/core/comp/script/script.h"
  "include/core/comp/script/builder.h"
  "include/core/comp/script/entry.h"
  "include/core/comp/sprite.h"
  "include/core/comp/transform.h"
  "include/core/sys/physics.h"
  "include/core/sys/render.h"
  "include/core/sys/scripts.h"
  "include/core/entity/builder.h"
  "include/core/util/file_reader.h"
  "src/util/file_reader.cpp")

target_include_directories(${CORE_LIB}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${CORE_LIB} PUBLIC raylib EnTT::EnTT sol2::sol2
                                         spdlog::spdlog)

add_subdirectory(test)
