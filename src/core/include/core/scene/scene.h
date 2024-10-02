#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <functional>

#include "core/sys/scripts.h"

namespace core::scene {

class SceneManager;

class Scene {
 public:
  using OnStartFunc = std::function<void(Scene &)>;
  using UpdateFunc = std::function<void(Scene &, float)>;

  Scene(SceneManager &scene_manager, OnStartFunc on_start,
        UpdateFunc update) noexcept;

  void OnStart() noexcept;
  void Update(float delta_time) noexcept;
  void Clear() noexcept;
  void Reset() noexcept;

  entt::registry &GetRegistry() noexcept;
  entt::dispatcher &GetDispatcher() noexcept;
  sys::ScriptSystem &GetScriptSystem() noexcept;

 private:
  entt::registry registry_;
  entt::dispatcher dispatcher_;
  sys::ScriptSystem script_system_;

  OnStartFunc on_start_;
  UpdateFunc update_;
};

}  // namespace core::scene
