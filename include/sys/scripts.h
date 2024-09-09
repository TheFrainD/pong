#pragma once

#include <entt/entity/registry.hpp>
#include <sol/sol.hpp>

namespace pong::sys {

class ScriptSystem {
 public:
  ScriptSystem();

  static void OnStart(entt::registry &registry);
  static void Update(entt::registry &registry, float delta_time);

  sol::state &GetState() noexcept { return state_; }

 private:
  void LoadInputModule();

  sol::state state_;
};

}  // namespace pong::sys