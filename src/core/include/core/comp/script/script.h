#pragma once

#include <string>
#include <unordered_map>

namespace core::comp {

struct ScriptComponent {
  std::unordered_map<std::string, int> scripts;
};

}  // namespace core::comp
