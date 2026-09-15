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
  std::string text = "tester miner 123 hello world";
  std::regex pattern(R"(\b\w\b)");

  for (std::sregex_iterator it(text.begin(), text.end(), pattern), end;
       it != end; ++it) {
    std::cout << it->str() << '\n';
  }

  return 0;
}
