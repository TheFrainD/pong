#include "comp/script.h"

namespace pong::comp {

namespace {
std::string SnakeCaseToPascalCase(const std::string &snake_case) {
  std::string pascal_case;
  bool capitalize_next = true;

  for (auto ch : snake_case) {
    if (ch == '_') {
      capitalize_next = true;
      continue;
    }

    if (capitalize_next) {
      pascal_case += std::toupper(ch);
      capitalize_next = false;
    } else {
      pascal_case += std::tolower(ch);
    }
  }

  return pascal_case;
}

std::string GetModuleName(const std::filesystem::path &path) noexcept {
  const auto filename = path.stem().string();
  return SnakeCaseToPascalCase(filename);
}
}  // namespace

void AddScript(entt::registry &registry, sys::ScriptSystem &script_system,
               entt::entity entity, const std::filesystem::path &path,
               const std::unordered_map<std::string, sol::object> &params) {
  const auto id = script_system.RegisterScript(path, params);
  const auto name = GetModuleName(path);

  if (registry.all_of<ScriptComponent>(entity)) {
    registry.get<ScriptComponent>(entity).scripts[name] = id;
    return;
  }

  registry.emplace<ScriptComponent>(entity).scripts[name] = id;
}

}  // namespace pong::comp