#pragma once

#include <entt/entity/registry.hpp>
#include <filesystem>
#include <sol/object.hpp>
#include <string>
#include <unordered_map>

#include "sys/scripts.h"

namespace pong::comp {

struct ScriptComponent {
  std::unordered_map<std::string, int> scripts;
};

void AddScript(entt::registry &registry, sys::ScriptSystem &script_system,
               entt::entity entity, const std::filesystem::path &path,
               const std::unordered_map<std::string, sol::object> &params = {});

}  // namespace pong::comp