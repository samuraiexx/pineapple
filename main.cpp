#include "debug.h"

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <set>

#include "tokenizer.h"
#include "tableCreator.h"
#include "paser.h"

using namespace std;

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

  code = "*a=b"; // db 
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

  cout << table << endl;

  Parser parser(table);
  parser.parseCode(x);

  return;
}
