#include <iostream>
#include <stack>
using namespace std;

class Solution {
public:
  bool isValid(string s) {
    stack<char> stack;
    for (char c : s) {
      switch (c) {
      case '(':
      case '{':
      case '[':
        stack.push(c);
        break;
      case ')':
        if (stack.empty() || stack.top() != '(')
          return false;
        stack.pop();
        break;
      case '}':
        if (stack.empty() || stack.top() != '{')
          return false;
        stack.pop();
        break;
      case ']':
        if (stack.empty() || stack.top() != '[')
          return false;
        stack.pop();
        break;
      default:
        break;
      }
    }
    return stack.empty();
  }
};

int main(int argc, char *argv[]) {
  Solution s;
  cout << s.isValid("()") << "\n";     // pass
  cout << s.isValid("()[]{}") << "\n"; // pass
  cout << s.isValid("(]") << "\n";     // fail
  cout << s.isValid("([])") << "\n";   // pass
  cout << s.isValid("([)]") << "\n";   // fail
  return 0;
}
