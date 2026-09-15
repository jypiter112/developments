#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Solution {
public:
  int lengthOfLongestSubstring(string s) {
    int n = s.length();
    int ans = 0;
    int i = 0, j = 0;

    vector<int> used_characters{};
    for (auto c : s) {
    }
  }
};

int main() {
  string substr = "abcabcbb";
  int len{Solution().lengthOfLongestSubstring(substr)};
  return 0;
}
