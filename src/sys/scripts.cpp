#include "sys/scripts.h"

#include <raylib.h>

#include "comp/name.h"
#include "comp/script.h"
#include "comp/sprite.h"
#include "comp/transform.h"

namespace pong::sys {

ScriptSystem::ScriptSystem(entt::registry &registry) {
  state_.open_libraries(sol::lib::base, sol::lib::package);

  RegisterComponents(registry);
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

  auto input = state_.create_table();
  input.set_function("IsKeyDown", &IsKeyDown);
  input["Key"] = key_codes;

  state_["package"]["loaded"]["Input"] = input;
}

void ScriptSystem::OnStart(entt::registry &registry) {
  auto view = registry.view<comp::Script>();
  for (const auto &entity : view) {
    auto &script = view.get<comp::Script>(entity);

    auto &env = script_envs_[script.id];
    if (!env.valid()) {
      continue;
    }
    SetContext(registry, env, entity);

    sol::function on_start = env["onStart"];
    if (on_start.valid()) {
      on_start();
    }
  }
}

void ScriptSystem::Update(entt::registry &registry, float delta_time) {
  auto view = registry.view<comp::Script>();
  for (const auto &entity : view) {
    auto &script = view.get<comp::Script>(entity);

    auto &env = script_envs_[script.id];
    if (!env.valid()) {
      continue;
    }

    sol::function update = env["update"];
    if (update.valid()) {
      update(delta_time);
    }
  }
}

void ScriptSystem::RegisterComponents(entt::registry &registry) {
  state_.new_usertype<Vector2>("Vector2", "x", &Vector2::x, "y", &Vector2::y);
  state_.new_usertype<comp::Transform>("Transform", "position",
                                       &comp::Transform::position);
  state_.new_usertype<comp::Sprite>("Sprite", "size", &comp::Sprite::size);

  state_.set_function("GetEntity",
                      [this, &registry](const std::string &name) -> sol::table {
                        auto entity = GetEntity(registry, name);
                        if (!entity.has_value()) {
                          return sol::nil;
                        }

                        return CreateLuaEntity(registry, *entity);
                      });
}

void ScriptSystem::RegisterSystemModule() {
  auto window = state_.create_table();
  window.set_function("GetWidth", &GetScreenWidth);
  window.set_function("GetHeight", &GetScreenHeight);

  auto system = state_.create_table();
  system["Window"] = window;

  state_["package"]["loaded"]["System"] = system;
}

std::optional<entt::entity> ScriptSystem::GetEntity(entt::registry &registry,
                                                    const std::string &name) {
  auto view = registry.view<comp::Name>();
  for (const auto &entity : view) {
    const auto &name_comp = view.get<comp::Name>(entity);
    if (name_comp.name == name) {
      return entity;
    }
  }
  return std::nullopt;
}
int ScriptSystem::RegisterScript(const std::filesystem::path &path) {
  sol::environment env(state_, sol::create, state_.globals());
  state_.script_file(path, env);
  script_envs_.push_back(env);
  return script_envs_.size() - 1;
}

sol::table ScriptSystem::CreateLuaEntity(entt::registry &registry,
                                         entt::entity entity) {
  auto lua_entity = state_.create_table();
  lua_entity["entity"] = entity;
  lua_entity.set_function(
      "GetComponent",
      [this, lua_entity,
       &registry](const std::string &component_name) -> sol::object {
        entt::entity this_entity = lua_entity["entity"];
        if (this_entity == entt::null) {
          return sol::nil;
        }

        return GetComponent(registry, this_entity, component_name);
      });

  lua_entity.set_function("GetName",
                          [this, lua_entity, &registry]() -> std::string {
                            sol::table ctx = state_["ctx"];
                            if (!ctx) {
                              return {};
                            }

                            entt::entity entity = ctx["entity"];

                            if (!registry.all_of<comp::Name>(entity)) {
                              return {};
                            }

                            return registry.get<comp::Name>(entity).name;
                          });

  return lua_entity;
}

sol::object ScriptSystem::GetComponent(entt::registry &registry,
                                       entt::entity entity,
                                       const std::string &name) {
  if (name == "Transform") {
    return sol::make_object(state_,
                            std::ref(registry.get<comp::Transform>(entity)));
  }

  if (name == "Sprite") {
    return sol::make_object(state_,
                            std::ref(registry.get<comp::Sprite>(entity)));
  }
  return sol::nil;
}

void ScriptSystem::SetContext(entt::registry &registry, sol::environment &env,
                              entt::entity entity) {
  env["self"] = CreateLuaEntity(registry, entity);

  //  for (const auto &[name, value] : params) {
  //    env[name] = value;
  //  }
}

}  // namespace pong::sys