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

    SetContext(entity, script);
    if (script.on_start.valid()) {
      script.on_start();
    }
  }
}

void ScriptSystem::Update(entt::registry &registry, float delta_time) {
  auto view = registry.view<comp::Script>();
  for (const auto &entity : view) {
    auto &script = view.get<comp::Script>(entity);

    SetContext(entity, script);
    if (script.update.valid()) {
      script.update(delta_time);
    }
  }
}

void ScriptSystem::SetContext(const entt::entity &entity,
                              const comp::Script &script) {
  auto ctx = state_.create_table();
  ctx["entity"] = entity;

  for (const auto &[name, value] : script.params) {
    ctx[name] = value;
  }

  state_["ctx"] = ctx;
}

void ScriptSystem::RegisterComponents(entt::registry &registry) {
  state_.new_usertype<Vector2>("Vector2", "x", &Vector2::x, "y", &Vector2::y);
  state_.new_usertype<comp::Transform>("Transform", "position",
                                       &comp::Transform::position);
  state_.new_usertype<comp::Sprite>("Sprite", "size", &comp::Sprite::size);

  auto get_component = [this, &registry](
                           entt::entity entity,
                           const std::string &name) -> sol::object {
    if (name == "Transform") {
      return sol::make_object(state_,
                              std::ref(registry.get<comp::Transform>(entity)));
    }

    if (name == "Sprite") {
      return sol::make_object(state_,
                              std::ref(registry.get<comp::Sprite>(entity)));
    }
    return sol::nil;
  };

  state_.set_function(
      "GetComponent",
      [get_component, this](const std::string &name) -> sol::object {
        sol::table ctx = state_["ctx"];
        if (!ctx) {
          return sol::nil;
        }

        entt::entity entity = ctx["entity"];

        return get_component(entity, name);
      });

  state_.set_function("GetName", [&]() -> std::string {
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

  state_.set_function(
      "GetEntity",
      [this, &registry, get_component](const std::string &name) -> sol::table {
        auto entity = GetEntity(registry, name);
        if (!entity.has_value()) {
          return sol::nil;
        }

        auto lua_entity = state_.create_table();
        lua_entity["entity"] = *entity;
        lua_entity.set_function(
            "GetComponent",
            [&, lua_entity,
             get_component](const std::string &component_name) -> sol::object {
              entt::entity this_entity = lua_entity["entity"];
              if (this_entity == entt::null) {
                return sol::nil;
              }

              return get_component(this_entity, component_name);
            });

        return lua_entity;
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

}  // namespace pong::sys