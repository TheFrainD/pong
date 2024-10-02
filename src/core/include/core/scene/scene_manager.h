#pragma once

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>
#include <memory>
#include <string>
#include <unordered_map>

#include "core/scene/scene.h"

namespace core::scene {

class SceneManager {
 public:
  std::shared_ptr<Scene> CreateScene(const std::string &name,
                                     Scene::OnStartFunc on_start,
                                     Scene::UpdateFunc update) noexcept;

  std::shared_ptr<Scene> GetCurrentScene() noexcept;

  void Transition(const std::string &name) noexcept;

  entt::registry &GetRegistry();
  entt::dispatcher &GetDispatcher();

 private:
  std::unordered_map<std::string, std::shared_ptr<core::scene::Scene>> scenes_;
  std::shared_ptr<Scene> current_scene_;
};

}  // namespace core::scene
