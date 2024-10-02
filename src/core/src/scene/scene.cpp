#include "core/scene/scene.h"

#include <utility>

#include "core/sys/scripts.h"

namespace core::scene {

Scene::Scene(SceneManager &scene_manager, OnStartFunc on_start,
             UpdateFunc update) noexcept
    : script_system_(registry_, dispatcher_, scene_manager),
      on_start_(std::move(on_start)),
      update_(std::move(update)) {}

void Scene::OnStart() noexcept {
  on_start_(*this);
  script_system_.OnStart();
}

void Scene::Update(float delta_time) noexcept {
  update_(*this, delta_time);
  script_system_.Update(delta_time);
}

void Scene::Reset() noexcept {
  Clear();
  OnStart();
}

void Scene::Clear() noexcept {
  registry_.clear();
  dispatcher_.clear();
}

entt::registry &Scene::GetRegistry() noexcept { return registry_; }

entt::dispatcher &Scene::GetDispatcher() noexcept { return dispatcher_; }

sys::ScriptSystem &Scene::GetScriptSystem() noexcept { return script_system_; }

}  // namespace core::scene
