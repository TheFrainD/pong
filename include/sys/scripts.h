#pragma once

#include <entt/entity/registry.hpp>
#include <sol/sol.hpp>

namespace pong::sys {

class ScriptSystem {
 public:
  explicit ScriptSystem(entt::registry &registry);

  void OnStart(entt::registry &registry);
  void Update(entt::registry &registry, float delta_time);

  sol::state &GetState() noexcept { return state_; }

 private:
  void LoadInputModule();

  void SetContext(const entt::entity &entity);

  sol::state state_;
};

}  // namespace pong::sys