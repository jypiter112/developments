#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::string text = "Contact alice@test.com or bob@example.org, "
                     "but not bob@example.com";

  /*
   * \b begin word boundayr (email composition)
   * \w+ any word
   * \. exactly special char .
   * [a-zA-Z]{2,}any characters a-Z and 2 or more characters
   * \b end of word boundary
   */
  std::regex pattern(R"(\b\w+[@]\w+\.[a-zA-Z]{2,}\b)");
  for (std::sregex_iterator it(text.begin(), text.end(), pattern), end;
       it != end; ++it) {
    std::cout << it->str() << "\n";
  }

  return 0;
}
