#include "core/sys/scripts.h"

#include <raylib.h>
#include <spdlog/spdlog.h>

#include <sol/error.hpp>
#include <sol/forward.hpp>
#include <sol/object.hpp>
#include <sol/types.hpp>

#include "core/comp/collider.h"
#include "core/comp/label.h"
#include "core/comp/name.h"
#include "core/comp/script/entry.h"
#include "core/comp/script/script.h"
#include "core/comp/sprite.h"
#include "core/comp/transform.h"
#include "core/scene/scene.h"
#include "core/scene/scene_manager.h"

namespace core::sys {

ScriptSystem::ScriptSystem(entt::registry &registry,
                           entt::dispatcher &dispatcher,
                           scene::SceneManager &scene_manager)
    : registry_(registry),
      dispatcher_(dispatcher),
      scene_manager_(scene_manager) {
  state_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

  RegisterComponents();
  RegisterSystemModule();
  RegisterInputModule();
  RegisterSceneManager();
}

void ScriptSystem::RegisterInputModule() {
  auto key_codes = state_.create_table();
  key_codes["Space"] = KEY_SPACE;
  key_codes["W"] = KEY_W;
  key_codes["S"] = KEY_S;
  key_codes["Up"] = KEY_UP;
  key_codes["Down"] = KEY_DOWN;
  key_codes["Enter"] = KEY_ENTER;
  key_codes["R"] = KEY_R;
  key_codes["Escape"] = KEY_ESCAPE;

  auto input = state_.create_table();
  input.set_function("IsKeyDown", &IsKeyDown);
  input["Key"] = key_codes;

  state_["package"]["loaded"]["Input"] = input;
}

void ScriptSystem::Init() {
  auto view = registry_.view<comp::ScriptComponent>();
  for (const auto &entity : view) {
    auto &script_component = view.get<comp::ScriptComponent>(entity);

    for (auto &[name, id] : script_component.scripts) {
      auto &entry = script_entries_[id];
      auto &env = entry.env;
      if (!env.valid()) {
        continue;
      }

      try {
        entry.env["self"] = CreateLuaEntity(entity);
      } catch (const sol::error &e) {
        spdlog::error("Failed to add lua entity: {}", e.what());
      }

      sol::function init = env["Init"];
      if (init.valid()) {
        init();
      }
    }
  }
}

void ScriptSystem::OnStart() {
  auto view = registry_.view<comp::ScriptComponent>();
  for (const auto &entity : view) {
    auto &script_component = view.get<comp::ScriptComponent>(entity);

    for (auto &[name, id] : script_component.scripts) {
      auto &entry = script_entries_[id];
      auto &env = entry.env;
      if (!env.valid()) {
        continue;
      }

      sol::function on_start = env["OnStart"];
      if (on_start.valid()) {
        on_start();
      }
    }
  }
}

void ScriptSystem::Update(float delta_time) {
  auto view = registry_.view<comp::ScriptComponent>();
  for (const auto &entity : view) {
    auto &script_component = view.get<comp::ScriptComponent>(entity);

    for (auto &[name, id] : script_component.scripts) {
      auto &lua_script = script_entries_[id];
      auto &env = lua_script.env;
      if (!env.valid()) {
        continue;
      }

      sol::function update = env["Update"];
      if (update.valid()) {
        sol::function_result result = update(delta_time);
        if (result.return_count() > 0 && !result.get<bool>(0)) {
          return;
        }
      }
    }
  }
}

void ScriptSystem::RegisterComponents() {
  state_.new_usertype<Vector2>("Vector2", "x", &Vector2::x, "y", &Vector2::y);
  state_.new_usertype<Color>("Color", "r", &Color::r, "g", &Color::g, "b",
                             &Color::b, "a", &Color::a);
  state_.new_usertype<comp::Transform>("Transform", "position",
                                       &comp::Transform::position);
  state_.new_usertype<comp::Sprite>("Sprite", "size", &comp::Sprite::size,
                                    "color", &comp::Sprite::color);
  state_.new_usertype<comp::Label>("Label", "text", &comp::Label::text,
                                   "fontSize", &comp::Label::font_size, "color",
                                   &comp::Label::color);

  state_.set_function("GetEntity",
                      [this](const std::string &name) -> sol::table {
                        auto entity = GetEntity(name);
                        if (!entity.has_value()) {
                          return sol::lua_nil;
                        }

                        return CreateLuaEntity(*entity);
                      });

  dispatcher_.sink<comp::CollisionEvent>()
      .connect<&ScriptSystem::HandleCollision>(this);
}

void ScriptSystem::RegisterSceneManager() {
  auto scene_manager = state_.create_table();
  scene_manager.set_function("ResetScene", [this]() {
    auto scene = scene_manager_.GetCurrentScene();
    if (scene != nullptr) {
      scene->Reset();
    }

    return sol::make_object(state_, false);
  });
  scene_manager.set_function("Transition", [this](const std::string &name) {
    scene_manager_.Transition(name);

    return sol::make_object(state_, false);
  });

  state_["package"]["loaded"]["SceneManager"] = scene_manager;
}

void ScriptSystem::RegisterSystemModule() {
  auto window = state_.create_table();
  window.set_function("GetWidth", &GetScreenWidth);
  window.set_function("GetHeight", &GetScreenHeight);

  auto system = state_.create_table();
  system["Window"] = window;

  state_["package"]["loaded"]["System"] = system;
}

std::optional<entt::entity> ScriptSystem::GetEntity(const std::string &name) {
  auto view = registry_.view<comp::Name>();
  for (const auto &entity : view) {
    const auto &name_comp = view.get<comp::Name>(entity);
    if (name_comp.name == name) {
      return entity;
    }
  }
  return std::nullopt;
}

int ScriptSystem::RegisterScript(comp::ScriptEntry entry) noexcept {
  script_entries_.push_back(std::move(entry));
  return script_entries_.size() - 1;
}

void ScriptSystem::HandleCollision(const comp::CollisionEvent &event) {
  if (registry_.all_of<comp::ScriptComponent>(event.a)) {
    auto script_component = registry_.get<comp::ScriptComponent>(event.a);

    for (auto &[name, id] : script_component.scripts) {
      auto &lua_script = script_entries_[id];
      auto &env = lua_script.env;
      if (!env.valid()) {
        return;
      }

      sol::function on_collision = env["OnCollision"];
      if (on_collision.valid()) {
        on_collision(CreateLuaEntity(event.b));
      }
    }
  }
}

sol::table ScriptSystem::CreateLuaEntity(entt::entity entity) {
  auto lua_entity = state_.create_table();
  lua_entity["entity"] = entity;
  lua_entity.set_function(
      "GetComponent",
      [this, lua_entity](const std::string &component_name) -> sol::object {
        entt::entity this_entity = lua_entity["entity"];
        if (this_entity == entt::null) {
          return sol::lua_nil;
        }

        return GetComponent(this_entity, component_name);
      });
  lua_entity.set_function("GetName", [this, lua_entity]() -> std::string {
    entt::entity this_entity = lua_entity["entity"];
    if (this_entity == entt::null) {
      return {};
    }

    if (!registry_.all_of<comp::Name>(this_entity)) {
      return {};
    }

    return registry_.get<comp::Name>(this_entity).name;
  });

  return lua_entity;
}

sol::object ScriptSystem::GetComponent(entt::entity entity,
                                       const std::string &name) {
  if (name == "Transform") {
    return sol::make_object(state_,
                            std::ref(registry_.get<comp::Transform>(entity)));
  }

  if (name == "Sprite") {
    return sol::make_object(state_,
                            std::ref(registry_.get<comp::Sprite>(entity)));
  }

  if (name == "Label") {
    return sol::make_object(state_,
                            std::ref(registry_.get<comp::Label>(entity)));
  }

  if (registry_.all_of<comp::ScriptComponent>(entity)) {
    auto &script_component = registry_.get<comp::ScriptComponent>(entity);
    if (script_component.scripts.contains(name)) {
      const auto id = script_component.scripts.at(name);
      return script_entries_[id].env;
    }
  }

  return sol::lua_nil;
}

comp::ScriptEntry &ScriptSystem::GetScript(const int id) {
  return script_entries_[id];
}

}  // namespace core::sys
