# Pong Clone

A simple Pong clone built as part of my game development journey.

## Features

- Basic 2D gameplay with paddles and a ball.
- Player vs. Player mode.

## Technologies

- **C++**
- **raylib**
- **entt**
- **sol2** (Lua binding)

## Requirements

- **CMake 3.22 or later** is required. You can download it from
  the [official CMake website](https://cmake.org/download/).
- **Conan 2.0 or later** You can install Conan by following the official
  guide [here](https://docs.conan.io/2/installation.html).

## Installation

1. **Clone the repository**:

    ```bash
    git clone --recursive https://github.com/TheFrainD/pong.git
    cd pong
    ```

2. **Build the project with CMake**:

    ```bash
    mkdir build && cd build
    cmake .. -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=third-party/cmake-conan/conan_provider.cmake -DCMAKE_BUILD_TYPE=Release
    cmake --build .
    ```

   *Note*: This project uses **cmake-conan** to manage dependencies. Ensure Conan 2.0+ is installed before building.

3. **Run the game**:

    ```bash
    ./pong
    ```

## License

This project is licensed under the [MIT License](LICENSE).