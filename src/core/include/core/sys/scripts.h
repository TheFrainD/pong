#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <filesystem>
#include <optional>
#include <sol/sol.hpp>
#include <utility>
#include <vector>

#include "core/comp/collider.h"
#include "core/util/file_reader.h"

namespace core::sys {

class ScriptSystem {
 public:
  struct ScriptEntry {
    sol::environment env;
    std::unordered_map<std::string, sol::object> params;

    ScriptEntry(sol::environment env,
                std::unordered_map<std::string, sol::object> params)
        : env(std::move(env)), params(std::move(params)) {}
  };

  ScriptSystem(entt::registry &registry, entt::dispatcher &dispatcher,
               util::FileReader file_reader = util::ReadFileToString);

  void OnStart();
  void Update(float delta_time);

  sol::state &GetState() noexcept { return state_; }

  int RegisterScript(
      const std::filesystem::path &path,
      const std::unordered_map<std::string, sol::object> &params);

  ScriptEntry &GetScript(int id);

 private:
  void RegisterComponents();
  void RegisterInputModule();
  void RegisterSystemModule();

  void SetContext(sol::environment &env,
                  const std::unordered_map<std::string, sol::object> &params,
                  entt::entity entity);
  sol::table CreateLuaEntity(entt::entity entity);
  sol::object GetComponent(entt::entity entity, const std::string &name);

  void HandleCollision(const comp::CollisionEvent &event);

  std::optional<entt::entity> GetEntity(const std::string &name);

  sol::state state_;
  std::vector<ScriptEntry> script_entries_;
  entt::registry &registry_;
  entt::dispatcher &dispatcher_;
  util::FileReader file_reader_;
};

}  // namespace core::sys
