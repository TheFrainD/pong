#pragma once

#include <set>
#include <stdexcept>
#include <string>

namespace core::comp {

struct Name {
  std::string name;

  explicit Name(const std::string &name) : name(name) {
    if (names_.contains(name)) {
      // TODO: Catch these issues
      throw std::runtime_error{"Entity with the name \"" + name +
                               "\" already exists!"};
    }
    names_.insert(name);
  }

 private:
  std::set<std::string> names_;
};

}  // namespace core::comp