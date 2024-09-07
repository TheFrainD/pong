#pragma once

#include <raylib.h>

namespace pong::comp {

// Sprite renderer component
// Only supports colored rectangles
struct SpriteRenderer {
  Vector2 size;
  Color color;
};

}  // namespace pong::comp