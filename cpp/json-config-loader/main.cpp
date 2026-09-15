#include <iostream>
#include <regex>
#include <string>

#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

class FileIOException : public std::runtime_error {
public:
  FileIOException(const fs::path &path, const std::string &reason)
      : std::runtime_error(
            std::format("File I/O error on '{}': {}", path.string(), reason)) {}
};

std::string mreadFile(const fs::path &path) {
  if (!fs::exists(path)) {
    throw FileIOException(path, "file doesnt exist");
  }

  auto stream =
      std::make_unique<std::ifstream>(path, std::ios::in | std::ios::binary);

  if (!stream->is_open()) {
    throw FileIOException(path, "unable to open stream");
  }
  // enables exceptions
  stream->exceptions(std::ifstream::badbit);

  std::string contents;
  try {
    const auto sz = fs::file_size(path);
    contents.resize(sz);
    stream->read(contents.data(), static_cast<std::streamsize>(sz));
  } catch (const std::ios_base::failure &e) {
    throw FileIOException(path, std::format("read failed {}", e.what()));
  }
  return contents;
}

int main(int argc, char **argv) {
  if (argc < 2)
    throw std::runtime_error(std::format("usage: {} <path>", argv[0]));
  try {
    std::string contents = mreadFile(argv[1]);
    std::cout << contents;
  } catch (const FileIOException &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
