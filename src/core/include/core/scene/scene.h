#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <functional>

#include "core/sys/scripts.h"

namespace core::scene {

class Scene {
 public:
  Scene(std::function<void(Scene &)> setup,
        std::function<void(Scene &, float)> update) noexcept;

  void Setup() noexcept;
  void OnStart() noexcept;
  void Update(float delta_time) noexcept;
  void Reset() noexcept;

  entt::registry &GetRegistry() noexcept;
  entt::dispatcher &GetDispatcher() noexcept;
  sys::ScriptSystem &GetScriptSystem() noexcept;

 private:
  entt::registry registry_;
  entt::dispatcher dispatcher_;
  sys::ScriptSystem script_system_;

  std::function<void(Scene &)> setup_;
  std::function<void(Scene &, float)> update_;
};

}  // namespace core::scene
