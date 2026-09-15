#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::string text = "Contact alice@test.com or bob@example.org, "
                     "but not bob@example.com";

  /*
   */
  std::regex pattern(R"(\b[a-zA-Z]+[@][a-zA-Z]+\.[a-zA-Z]{2,}\b)");
  for (std::sregex_iterator it(text.begin(), text.end(), pattern), end;
       it != end; ++it) {
    std::cout << it->str() << "\n";
  }

  return 0;
}
