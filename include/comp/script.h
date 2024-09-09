#pragma once

#include <filesystem>
#include <sol/function.hpp>
#include <sol/state.hpp>

namespace pong::comp {

struct Script {
  Script(sol::state &state, const std::filesystem::path &path);

  sol::function on_start_;
  sol::function update_;
};

}  // namespace pong::comp