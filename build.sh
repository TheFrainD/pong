#!/bin/bash

# Exit on error
set -e 

echo "Building pong..."

if [[ ! -d build ]]; then
  echo "Creating build directory..."
  mkdir -p build
fi

cd build

echo "Generating CMake project..."
cmake .. -G Ninja \
  -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=third-party/cmake-conan/conan_provider.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "Building project..."
ninja

# Run tests
ctest --output-on-failure

echo "Success!"

