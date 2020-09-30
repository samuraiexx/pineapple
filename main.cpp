#include "debug.h"

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <set>

#include "tokenizer.h"
#include "tableCreator.h"

using namespace std;

void printTable(const vector<vector<int>>& table);

void main(int argc, char** argv) {
  Tokenizer tokenizer;

  ifstream t("file.txt");
  string code;
  
  if (argc == 1) {
    code = R"(
      type T1 = array[2] of integer;
      type T2 = struct { s : string };

      var x, y, z: integer;
      var c : char;
      var s : string;
      x = 1;
      y = 2;
      z = 3;

      c = 'a';
      s = "a few words";
      x = y + z;

      if (x > 1) {
        y++;
        z = y * z;
      }

      while(x < 10 && y > 1){
        x++;
        z++;
        if(z > 5) {
          break;
        }
      }

      function f(a: integer): char {
      }
    )";
  } else {
    ifstream t(argv[1]);
    code = string(istreambuf_iterator<char>(t), istreambuf_iterator<char>());
  }

  code.push_back('\x03'); // End of Text
  const auto x = tokenizer.tokenizeCode(code);

  /** Test Code for Tokenizer
   ** 
  for (auto t : x) {
    cout << '(' << token2String(t.primaryToken) << ", " << t.secondaryToken << ')' << endl;
  }
  **/
  TableCreator tableCreator;
  auto table = tableCreator.getTable();

  printTable(table);
}

void printTable(const vector<vector<int>>& table) {
  set<int> skipColumn;

  for (int j = 0; table.size() && j < table[0].size(); j++) {
    bool onlyZeros = true;
    for (int i = 0; i < table.size(); i++) {
      onlyZeros = onlyZeros && table[i][j] == 0;
    }
    if(onlyZeros) skipColumn.insert(j);
  }

  for (int i = 0; i < table.size(); i++) {
    for (int j = 0; j < table[0].size(); j++) {
      if (skipColumn.count(j)) {
        continue;
      }
      cout << table[i][j] << '\t';
    }
    cout << endl;
  }
}
