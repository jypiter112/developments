#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

class FileIOException : public std::runtime_error {
public:
  FileIOException(const fs::path &path, const std::string &reason)
      : std::runtime_error(
            std::format("File I/O error on '{}': {}", path.string(), reason)) {}
};

struct PasswordEntry {
  string username;
  string password;
};

class PasswordManager {
public:
  void LoadDatabase(const fs::path &path) {
    string contents = _readFile(path);
    if (contents.length() < 1)
      throw FileIOException(path, "file read failed\n");
    std::regex pattern(R"(\b(\w+):(\w+)\b)");
    int entry_count{0};
    for (std::sregex_iterator it(contents.begin(), contents.end(), pattern),
         end;
         it != end; ++it) {
      std::smatch match = *it;
      if (!match.empty()) {
        PasswordEntry pe;
        pe.username = match[1].str();
        pe.password = match[2].str();
        _database.emplace(match[1].str(), pe);
      }
    }
    if (_database.empty())
      throw runtime_error("failed to load database, or database empty.\n");
    else
      cout << "db loaded ok\n";
  }

private:
  std::string sha256hash(std::string_view input) {
    std::string hash(input);
    return hash;
  }
  std::string xorEncrypt(std::string input, int key) {
    for (int i{0}; i < input.length(); i++) {
      input[i] = input[i] << key;
    }
    return input;
  }
  std::string _readFile(const fs::path &path) {
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

    std::string contents{};
    try {
      const auto sz = fs::file_size(path);
      contents.resize(sz);
      stream->read(contents.data(), static_cast<std::streamsize>(sz));
    } catch (const std::ios_base::failure &e) {
      throw FileIOException(path, std::format("read failed {}", e.what()));
    }
    return contents;
  }
  std::unordered_map<std::string, PasswordEntry> _database;
};

int main(int argc, char *argv[]) {
  auto passman = make_unique<PasswordManager>();
  passman->LoadDatabase("maindb.db");
}
