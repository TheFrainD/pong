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

  int RegisterScript(
      const std::filesystem::path &path,
      const std::unordered_map<std::string, sol::object> &params);

 private:
  struct Script {
    sol::environment env;
    std::unordered_map<std::string, sol::object> params;
  };

  void RegisterComponents(entt::registry &registry);
  void RegisterInputModule();
  void RegisterSystemModule();

  void SetContext(entt::registry &registry, sol::environment &env,
                  const std::unordered_map<std::string, sol::object> &params,
                  entt::entity entity);
  sol::table CreateLuaEntity(entt::registry &registry, entt::entity entity);
  sol::object GetComponent(entt::registry &registry, entt::entity entity,
                           const std::string &name);

  static std::optional<entt::entity> GetEntity(entt::registry &registry,
                                               const std::string &name);

  sol::state state_;
  std::vector<Script> script_envs_;
};

}  // namespace pong::sys