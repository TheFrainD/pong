#pragma once

#include <sol/environment.hpp>
#include <sol/state.hpp>
#include <string>

namespace core::comp {

struct ScriptEntry {
  std::string name;
  sol::state &state;
  sol::environment env;
};

}  // namespace core::comp
