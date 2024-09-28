#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <optional>
#include <sol/sol.hpp>
#include <vector>

#include "core/comp/collider.h"
#include "core/comp/script/entry.h"

namespace core::sys {

class ScriptSystem {
 public:
  ScriptSystem(entt::registry &registry, entt::dispatcher &dispatcher);

  void OnStart();
  void Update(float delta_time);

  sol::state &GetState() noexcept { return state_; }

  int RegisterScript(comp::ScriptEntry entry) noexcept;

  comp::ScriptEntry &GetScript(int id);

 private:
  void RegisterComponents();
  void RegisterInputModule();
  void RegisterSystemModule();

  sol::table CreateLuaEntity(entt::entity entity);
  sol::object GetComponent(entt::entity entity, const std::string &name);

  void HandleCollision(const comp::CollisionEvent &event);

  std::optional<entt::entity> GetEntity(const std::string &name);

  sol::state state_;
  std::vector<comp::ScriptEntry> script_entries_;
  entt::registry &registry_;
  entt::dispatcher &dispatcher_;
};

}  // namespace core::sys
