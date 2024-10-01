#include "core/scene/scene.h"

#include <utility>

#include "core/sys/scripts.h"

namespace core::scene {

Scene::Scene(std::function<void(Scene &)> setup,
             std::function<void(Scene &, float)> update) noexcept
    : script_system_(registry_, dispatcher_),
      setup_(std::move(setup)),
      update_(std::move(update)) {}

void Scene::Setup() noexcept { setup_(*this); }

void Scene::OnStart() noexcept { script_system_.OnStart(); }

void Scene::Update(float delta_time) noexcept {
  update_(*this, delta_time);
  script_system_.Update(delta_time);
}

void Scene::Reset() noexcept {
  registry_.clear();
  dispatcher_.clear();
  Setup();
}

entt::registry &Scene::GetRegistry() noexcept { return registry_; }

entt::dispatcher &Scene::GetDispatcher() noexcept { return dispatcher_; }

sys::ScriptSystem &Scene::GetScriptSystem() noexcept { return script_system_; }

}  // namespace core::scene
