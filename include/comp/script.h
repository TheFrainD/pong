#pragma once

#include <filesystem>
#include <sol/function.hpp>
#include <sol/state.hpp>
#include <unordered_map>

namespace pong::comp {

struct Script {
  Script(sol::state &state, const std::filesystem::path &path,
         const std::unordered_map<std::string, sol::object> &params = {});

  sol::function on_start;
  sol::function update;
  std::unordered_map<std::string, sol::object> params;
};

}  // namespace pong::comp