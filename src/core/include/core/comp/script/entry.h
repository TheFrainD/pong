#pragma once

#include <sol/environment.hpp>
#include <sol/state.hpp>

namespace core::comp {

struct ScriptEntry {
  sol::state &state;
  sol::environment env;
};

}  // namespace core::comp
