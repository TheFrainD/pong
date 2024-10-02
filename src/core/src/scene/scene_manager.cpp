#include "core/scene/scene_manager.h"

#include <spdlog/spdlog.h>

#include <memory>
#include <utility>

#include "core/scene/scene.h"

namespace core::scene {

std::shared_ptr<Scene> SceneManager::CreateScene(
    const std::string &name, Scene::OnStartFunc on_start,
    Scene::UpdateFunc update) noexcept {
  auto scene = scenes_.emplace(
      name,
      std::make_shared<Scene>(*this, std::move(on_start), std::move(update)));
  return (*scene.first).second;
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene() noexcept {
  return current_scene_;
}

void SceneManager::Transition(const std::string &name) noexcept {
  if (!scenes_.contains(name)) {
    spdlog::warn("There is no scene named '{}'! Transition failed!", name);
    return;
  }

  if (current_scene_ != nullptr) {
    current_scene_->Reset();
  }

  auto scene = scenes_.at(name);
  scene->OnStart();
  current_scene_ = scene;
}

entt::registry &SceneManager::GetRegistry() {
  return GetCurrentScene()->GetRegistry();
}

entt::dispatcher &SceneManager::GetDispatcher() {
  return GetCurrentScene()->GetDispatcher();
}

}  // namespace core::scene
