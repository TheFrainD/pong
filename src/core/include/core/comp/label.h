#pragma once

#include <raylib.h>

#include <string>

namespace core::comp {

struct Label {
  std::string text;
  int font_size;
  Color color;
};

}  // namespace core::comp