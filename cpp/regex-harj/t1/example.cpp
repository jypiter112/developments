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
  std::string text = "cat catalog dog category scatter";
  std::regex pattern(R"(\bcat\w*)");

  for (std::sregex_iterator it(text.begin(), text.end(), pattern), end;
       it != end; ++it) {
    std::cout << it->str() << '\n';
  }

  return 0;
}
