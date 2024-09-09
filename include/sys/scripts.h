#pragma once

#include <entt/entity/registry.hpp>
#include <optional>
#include <sol/sol.hpp>

#include "comp/script.h"

namespace pong::sys {

class ScriptSystem {
 public:
  explicit ScriptSystem(entt::registry &registry);

  void OnStart(entt::registry &registry);
  void Update(entt::registry &registry, float delta_time);

  sol::state &GetState() noexcept { return state_; }

 private:
  void RegisterComponents(entt::registry &registry);
  void RegisterInputModule();
  void RegisterSystemModule();

  void SetContext(const entt::entity &entity, const comp::Script &script);

  static std::optional<entt::entity> GetEntity(entt::registry &registry,
                                               const std::string &name);

  sol::state state_;
};

}  // namespace pong::sys