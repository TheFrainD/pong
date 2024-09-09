#include "comp/script.h"

namespace pong::comp {
Script::Script(sol::state &state, const std::filesystem::path &path) {
  state.script_file(path);

  on_start_ = state["onStart"];
  update_ = state["update"];
}
}  // namespace pong::comp