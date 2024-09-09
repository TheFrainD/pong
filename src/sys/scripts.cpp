#include "sys/scripts.h"

#include <raylib.h>

#include "comp/script.h"

namespace pong::sys {

ScriptSystem::ScriptSystem() {
  state_.open_libraries(sol::lib::base, sol::lib::package);

  LoadInputModule();
}

void ScriptSystem::LoadInputModule() {
  auto input = state_.create_table();
  input.set_function("IsKeyDown", &IsKeyDown);

  auto key_codes = state_.create_table();
  key_codes["Space"] = KEY_SPACE;
  input["Key"] = key_codes;

  state_["package"]["loaded"]["Input"] = input;
}

void ScriptSystem::OnStart(entt::registry &registry) {
  auto view = registry.view<comp::Script>();
  view.each([](const auto &script) {
    if (script.on_start_.valid()) {
      script.on_start_();
    }
  });
}

void ScriptSystem::Update(entt::registry &registry, float delta_time) {
  auto view = registry.view<comp::Script>();
  view.each([delta_time](const auto &script) {
    if (script.update_.valid()) {
      script.update_(delta_time);
    }
  });
}

}  // namespace pong::sys