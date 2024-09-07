#pragma once

#include <raylib.h>

namespace pong::comp {

// Only supports colored rectangles
struct Sprite {
  Vector2 size;
  Color color;
};

}  // namespace pong::comp