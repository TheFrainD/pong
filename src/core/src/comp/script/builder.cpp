#include "core/comp/script/builder.h"

#include <spdlog/spdlog.h>

#include <optional>
#include <sol/error.hpp>
#include <sol/types.hpp>

#include "core/util/file_reader.h"

namespace {

std::string SnakeCaseToPascalCase(const std::string &snake_case) {
  std::string pascal_case;
  bool capitalize_next = true;

  for (auto ch : snake_case) {
    if (ch == '_') {
      capitalize_next = true;
      continue;
    }

    if (capitalize_next) {
      pascal_case += std::toupper(ch);
      capitalize_next = false;
    } else {
      pascal_case += std::tolower(ch);
    }
  }

  return pascal_case;
}

std::string GetModuleName(const std::filesystem::path &path) noexcept {
  const auto filename = path.stem().string();
  return SnakeCaseToPascalCase(filename);
}

}  // namespace

namespace core::comp {

ScriptBuilder::ScriptBuilder(ScriptEntry &&entry) noexcept
    : entry_(std::move(entry)) {}

std::optional<ScriptBuilder> ScriptBuilder::Create(const std::string &name,
                                                   const std::string &script,
                                                   sol::state &state) noexcept {
  try {
    auto env = sol::environment(state, sol::create, state.globals());
    state.script(script, env);
    return ScriptBuilder({name, state, std::move(env)});
  } catch (const sol::error &e) {
    spdlog::error("Failed to create Lua environment: {}", e.what());
    return std::nullopt;
  }
}

std::optional<ScriptBuilder> ScriptBuilder::Create(
    const std::filesystem::path &path, sol::state &state) noexcept {
  const auto name = GetModuleName(path);
  try {
    const auto script = core::util::ReadFileToString(path);
    return Create(name, script, state);
  } catch (const core::util::FileReaderError &e) {
    spdlog::error("Failed to read script file '{}': {}", path.string(),
                  e.what());
    return std::nullopt;
  }
}

ScriptEntry &&ScriptBuilder::GetEntry() noexcept { return std::move(entry_); }

}  // namespace core::comp
