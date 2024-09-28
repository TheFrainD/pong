#include "core/util/file_reader.h"

#include <fstream>
#include <sstream>

namespace core::util {

std::string ReadFileToString(const std::filesystem::path &path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    throw FileReaderError{"Couldn't open file: " + path.string()};
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

}  // namespace core::util