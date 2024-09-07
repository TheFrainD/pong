#pragma once

namespace pong::comp {

struct Paddle {
  enum class Side { kLeft, kRight };

  Side side;
};

}  // namespace pong::comp