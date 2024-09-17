#pragma once

#include <filesystem>
#include <functional>
#include <stdexcept>
#include <string>

namespace core::util {

using FileReader = std::function<std::string(const std::filesystem::path &)>;

struct FileReaderError : public std::runtime_error {
  explicit FileReaderError(const std::string &what_arg)
      : std::runtime_error(what_arg) {}
};

std::string ReadFileToString(const std::filesystem::path &path);

}  // namespace core::util