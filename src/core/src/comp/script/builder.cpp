#include "core/comp/script/builder.h"

#include <optional>
#include <sol/error.hpp>
#include <sol/types.hpp>

namespace core::comp {

ScriptBuilder::ScriptBuilder(ScriptEntry &&entry) noexcept
    : entry_(std::move(entry)) {}

std::optional<ScriptBuilder> ScriptBuilder::Create(const std::string &script,
                                                   sol::state &state) noexcept {
  try {
    auto env = sol::environment(state, sol::create, state.globals());
    state.script(script, env);
    return ScriptBuilder({state, std::move(env)});
  } catch (const sol::error &e) {
    spdlog::error("Failed to create Lua environment: {}", e.what());
    return std::nullopt;
  }
}

ScriptEntry &&ScriptBuilder::GetEntry() noexcept { return std::move(entry_); }

}  // namespace core::comp
