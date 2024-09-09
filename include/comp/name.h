#pragma once

#include <set>
#include <stdexcept>
#include <string>

namespace pong::comp {

struct Name {
  std::string name;

  explicit Name(const std::string &name) : name(name) {
    if (names_.contains(name)) {
      throw std::runtime_error{"Entity with the name \"" + name +
                               "\" already exists!"};
    }
    names_.insert(name);
  }

 private:
  std::set<std::string> names_;
};

}  // namespace pong::comp