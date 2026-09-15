#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::string logs = "[INFO] Server started\n"
                     "[WARNING] Database connection failed\n"
                     "[DEBUG] Cache checked\n"
                     "[ERROR] Timeout occurred\n";

  std::regex pattern(R"(((\[ERROR\]|\[WARNING\])\s*(.*)))");

  for (std::sregex_iterator it(logs.begin(), logs.end(), pattern), end;
       it != end; ++it) {
    std::cout << "Error: " << (*it)[1].str() << '\n';
  }
}
