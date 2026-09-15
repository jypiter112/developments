#include <iostream>
#include <regex>
#include <string>
/*
 * Find worst that start with cat
 *
 *    Output:
 * cat
 * catalog
 * category
 */
int main(int argc, char *argv[]) {
  std::string text = "runnign jumping run jumped eating";
  /*
   * \b word boundary
   * \w+ one or more word characters
   * ing <pattern>
   * \b end of word boundary
   */
  std::regex pattern(R"(\b\w+ing\b)");

  for (std::sregex_iterator it(text.begin(), text.end(), pattern), end;
       it != end; ++it) {
    std::cout << it->str() << '\n';
  }

  return 0;
}
