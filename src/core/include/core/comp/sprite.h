#pragma once

#include <raylib.h>

namespace core::comp {

// Only supports colored rectangles
struct Sprite {
  Vector2 size;
  Color color;
};

}  // namespace core::comp