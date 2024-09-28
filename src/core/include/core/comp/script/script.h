#pragma once

#include <entt/entity/registry.hpp>
#include <sol/object.hpp>
#include <string>
#include <unordered_map>

#include "core/comp/script/entry.h"
#include "core/sys/scripts.h"

namespace core::comp {

struct ScriptComponent {
  std::unordered_map<std::string, int> scripts;
};

std::optional<ScriptComponent> AddScript(entt::registry &registry,
                                         sys::ScriptSystem &script_system,
                                         entt::entity entity,
                                         const std::string &name,
                                         ScriptEntry entry) noexcept;

}  // namespace core::comp
