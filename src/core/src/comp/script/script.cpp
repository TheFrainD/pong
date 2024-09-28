#include "core/comp/script/script.h"

#include <spdlog/spdlog.h>

#include <optional>

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

/*std::string GetModuleName(const std::filesystem::path &path) noexcept {*/
/*  const auto filename = path.stem().string();*/
/*  return SnakeCaseToPascalCase(filename);*/
/*}*/

}  // namespace

namespace core::comp {

std::optional<ScriptComponent> AddScript(entt::registry &registry,
                                         sys::ScriptSystem &script_system,
                                         entt::entity entity,
                                         const std::string &name,
                                         ScriptEntry entry) noexcept {
  const auto id = script_system.RegisterScript(std::move(entry));
  if (id == -1) {
    return std::nullopt;
  }

  if (registry.all_of<ScriptComponent>(entity)) {
    auto &comp = registry.get<ScriptComponent>(entity);
    comp.scripts[SnakeCaseToPascalCase(name)] = id;
    return comp;
  }

  auto &comp = registry.emplace<ScriptComponent>(entity);
  comp.scripts[SnakeCaseToPascalCase(name)] = id;
  return comp;
}

}  // namespace core::comp
