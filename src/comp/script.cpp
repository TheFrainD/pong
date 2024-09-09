#include "comp/script.h"

namespace pong::comp {
Script::Script(sol::state &state, const std::filesystem::path &path,
               const std::unordered_map<std::string, sol::object> &params)
    : params(params) {
  state.script_file(path);

  on_start = state["onStart"];
  update = state["update"];
}
}  // namespace pong::comp