#include <iostream>
#include <regex>
#include <string>

int main(int argc, char *argv[]) {
  std::string url = "https://www.example.com/some/long/page.php";

  std::regex pattern(R"(\b((*\c+)\.{\/\/}(w*)\.\w+\.\w+)\b)");

  for (std::sregex_iterator it(url.begin(), url.end(), pattern), end; it != end;
       ++it) {
    std::cout << it->str() << '\n';
  }
}
