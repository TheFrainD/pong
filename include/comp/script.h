#pragma once

#include <filesystem>
#include <sol/object.hpp>
#include <unordered_map>

#include "sys/scripts.h"

namespace pong::comp {

struct Script {
  explicit Script(
      sys::ScriptSystem &script_system, const std::filesystem::path &path,
      const std::unordered_map<std::string, sol::object> &params = {})
      : id(script_system.RegisterScript(path, params)) {}

  int id;
};

}  // namespace pong::comp