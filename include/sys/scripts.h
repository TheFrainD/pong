#pragma once

#include <entt/entity/registry.hpp>
#include <filesystem>
#include <optional>
#include <sol/sol.hpp>
#include <vector>

namespace pong::sys {

class ScriptSystem {
 public:
  explicit ScriptSystem(entt::registry &registry);

  void OnStart(entt::registry &registry);
  void Update(entt::registry &registry, float delta_time);

  sol::state &GetState() noexcept { return state_; }

  int RegisterScript(const std::filesystem::path &path);

 private:
  void RegisterComponents(entt::registry &registry);
  void RegisterInputModule();
  void RegisterSystemModule();

  void SetContext(entt::registry &registry, sol::environment &env,
                  entt::entity entity);
  sol::table CreateLuaEntity(entt::registry &registry, entt::entity entity);
  sol::object GetComponent(entt::registry &registry, entt::entity entity,
                           const std::string &name);

  static std::optional<entt::entity> GetEntity(entt::registry &registry,
                                               const std::string &name);

  sol::state state_;
  std::vector<sol::environment> script_envs_;
};

}  // namespace pong::sys