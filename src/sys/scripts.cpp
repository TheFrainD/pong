#include "sys/scripts.h"

#include <raylib.h>

#include "comp/script.h"
#include "comp/transform.h"

pong::comp::Transform GetTransform(entt::registry &registry,
                                   entt::entity entity) {
  return registry.get<pong::comp::Transform>(entity);
}

namespace pong::sys {

ScriptSystem::ScriptSystem(entt::registry &registry) {
  state_.open_libraries(sol::lib::base, sol::lib::package);

  state_.new_usertype<Vector2>("Vector2", "x", &Vector2::x, "y", &Vector2::y);
  state_.new_usertype<comp::Transform>("Transform", "position",
                                       &comp::Transform::position);

  state_.set_function(
      "GetComponent", [&](const std::string &name) -> sol::object {
        sol::table ctx = state_["ctx"];
        if (!ctx) {
          return sol::nil;
        }

        entt::entity entity = ctx["entity"];

        if (name == "Transform") {
          return sol::make_object(
              state_, std::ref(registry.get<comp::Transform>(entity)));
        }
        return sol::nil;
      });

  LoadInputModule();
}

void ScriptSystem::LoadInputModule() {
  auto input = state_.create_table();
  input.set_function("IsKeyDown", &IsKeyDown);

  auto key_codes = state_.create_table();
  key_codes["Space"] = KEY_SPACE;
  key_codes["W"] = KEY_W;
  key_codes["S"] = KEY_S;
  input["Key"] = key_codes;

  state_["package"]["loaded"]["Input"] = input;
}

void ScriptSystem::OnStart(entt::registry &registry) {
  auto view = registry.view<comp::Script>();
  for (const auto &entity : view) {
    auto &script = view.get<comp::Script>(entity);

    SetContext(entity);
    if (script.on_start_.valid()) {
      script.on_start_();
    }
  }
}

void ScriptSystem::Update(entt::registry &registry, float delta_time) {
  auto view = registry.view<comp::Script>();
  for (const auto &entity : view) {
    auto &script = view.get<comp::Script>(entity);

    SetContext(entity);
    if (script.update_.valid()) {
      script.update_(delta_time);
    }
  }
}

void ScriptSystem::SetContext(const entt::entity &entity) {
  auto ctx = state_.create_table();
  ctx["entity"] = entity;
  state_["ctx"] = ctx;
}

}  // namespace pong::sys