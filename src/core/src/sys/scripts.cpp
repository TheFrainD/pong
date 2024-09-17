#include "core/sys/scripts.h"

#include <raylib.h>

#include "core/comp/collider.h"
#include "core/comp/label.h"
#include "core/comp/name.h"
#include "core/comp/script.h"
#include "core/comp/sprite.h"
#include "core/comp/transform.h"

namespace core::sys {

ScriptSystem::ScriptSystem(entt::registry &registry,
                           entt::dispatcher &dispatcher,
                           const util::FileReader &file_reader)
    : registry_(registry), dispatcher_(dispatcher), file_reader_(file_reader) {
  state_.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);

  RegisterComponents();
  RegisterSystemModule();
  RegisterInputModule();
}

void ScriptSystem::RegisterInputModule() {
  auto key_codes = state_.create_table();
  key_codes["Space"] = KEY_SPACE;
  key_codes["W"] = KEY_W;
  key_codes["S"] = KEY_S;
  key_codes["Up"] = KEY_UP;
  key_codes["Down"] = KEY_DOWN;
  key_codes["Enter"] = KEY_ENTER;

  auto input = state_.create_table();
  input.set_function("IsKeyDown", &IsKeyDown);
  input["Key"] = key_codes;

  state_["package"]["loaded"]["Input"] = input;
}

void ScriptSystem::OnStart() {
  auto view = registry_.view<comp::ScriptComponent>();
  for (const auto &entity : view) {
    auto &script_component = view.get<comp::ScriptComponent>(entity);

    for (auto &[name, id] : script_component.scripts) {
      auto &lua_script = script_envs_[id];
      auto &env = lua_script.env;
      if (!env.valid()) {
        continue;
      }
      SetContext(env, lua_script.params, entity);

      sol::function on_start = env["onStart"];
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
      auto &lua_script = script_envs_[id];
      auto &env = lua_script.env;
      if (!env.valid()) {
        continue;
      }

      sol::function update = env["update"];
      if (update.valid()) {
        update(delta_time);
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
                          return sol::nil;
                        }

                        return CreateLuaEntity(*entity);
                      });

  dispatcher_.sink<comp::CollisionEvent>()
      .connect<&ScriptSystem::HandleCollision>(this);
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
int ScriptSystem::RegisterScript(
    const std::filesystem::path &path,
    const std::unordered_map<std::string, sol::object> &params) {
  sol::environment env(state_, sol::create, state_.globals());
  auto script = file_reader_(path);
  state_.script(script, env);
  script_envs_.emplace_back(env, params);
  return script_envs_.size() - 1;
}

void ScriptSystem::SetContext(
    sol::environment &env,
    const std::unordered_map<std::string, sol::object> &params,
    entt::entity entity) {
  env["self"] = CreateLuaEntity(entity);

  for (const auto &[name, value] : params) {
    env[name] = value;
  }
}

void ScriptSystem::HandleCollision(const comp::CollisionEvent &event) {
  if (registry_.all_of<comp::ScriptComponent>(event.a)) {
    auto script_component = registry_.get<comp::ScriptComponent>(event.a);

    for (auto &[name, id] : script_component.scripts) {
      auto &lua_script = script_envs_[id];
      auto &env = lua_script.env;
      if (!env.valid()) {
        return;
      }

      sol::function on_collision = env["onCollision"];
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
          return sol::nil;
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
      return script_envs_[id].env;
    }
  }

  return sol::nil;
}

}  // namespace core::sys