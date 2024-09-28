#pragma once

#include <spdlog/spdlog.h>

#include <filesystem>
#include <optional>
#include <sol/error.hpp>
#include <sol/state.hpp>

#include "core/comp/script/entry.h"

namespace core::comp {

class ScriptBuilder {
 public:
  static std::optional<ScriptBuilder> Create(const std::string &name,
                                             const std::string &script,
                                             sol::state &state) noexcept;

  static std::optional<ScriptBuilder> Create(const std::filesystem::path &path,
                                             sol::state &state) noexcept;
  template <typename T, typename... Args>
  ScriptBuilder &AddParameter(const std::string &name,
                              Args &&...args) noexcept {
    try {
      entry_.env[name] =
          sol::make_object(entry_.state, T(std::forward<Args>(args)...));
    } catch (const sol::error &e) {
      spdlog::error("Failed to add parameter '{}': {}", name, e.what());
    }

    return *this;
  }

  template <typename T>
  ScriptBuilder &AddParameter(const std::string &name,
                              const T &value) noexcept {
    try {
      entry_.env[name] = sol::make_object(entry_.state, value);
    } catch (const sol::error &e) {
      spdlog::error("Failed to add parameter '{}': {}", name, e.what());
    }

    return *this;
  }

  ScriptEntry &&GetEntry() noexcept;

 private:
  explicit ScriptBuilder(ScriptEntry &&entry) noexcept;

  ScriptEntry entry_;
};

}  // namespace core::comp
