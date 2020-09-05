#include <string>
#include <iostream>

#include "tokenizer.h"

using namespace std;

int main() {
  Tokenizer tokenizer;

  string code = R"(
  int x = 10;
  function f() {
    char z = '1';
    string s = "string";
  }
)";
  code.push_back('\x03'); // End of Text

  const auto x = tokenizer.tokenizeString(code);

  for (auto t : x) {
    cout << (int)t.first << ", " << t.second << endl;
  }

  return 0;
}