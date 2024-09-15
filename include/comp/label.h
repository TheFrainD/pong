#pragma once

#include <raylib.h>

#include <string>

namespace pong::comp {

struct Label {
  std::string text;
  int font_size;
  Color color;
};

}  // namespace pong::comp