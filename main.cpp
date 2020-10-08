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

bool getBoolFlag(int argc, char** argv, string flag);

void main(int argc, char** argv) {
  Tokenizer tokenizer;

  string codePath = "code.txt";
  bool recalcTable = getBoolFlag(argc, argv, "RECALC_TABLE");
  
  if (argc == 1) {
    cout << "This will be executed with the file code.txt, please pass the code file as an argument, f.e.: ./ExxCompiler file.txt\n";
    cout << "You can also use the flag RECALC_TABLE to reprocess the action table\n";
  } else if(!recalcTable || recalcTable && argc > 3) {
    codePath = argv[1];
  }

  ifstream p("banner.txt");
  string banner = string(istreambuf_iterator<char>(p), istreambuf_iterator<char>());
  cout << banner << endl;
  
  cout << "Running Pineapple compiler for code " << codePath << endl;

  ifstream t(codePath);
  string code = string(istreambuf_iterator<char>(t), istreambuf_iterator<char>());
  code.push_back('\x03'); // End of Text

  cout << "Generating tokens... ";
  const auto tokens = tokenizer.tokenizeCode(code);
  cout << "Done" << endl;

  cout << "Creating action table... ";
  TableCreator tableCreator(recalcTable);
  auto table = tableCreator.getTable();
  cout << "Done" << endl;

  // cout << table << endl;

  cout << "Parsing code... ";
  Parser parser(table);
  parser.parseCode(tokens);
  cout << "Done" << endl;

  return;
}

bool getBoolFlag(int argc, char** argv, string flag) {
  for (int i = 0; i < argc; i++) {
    if (string(argv[i]) == flag) {
      return true;
    }
  }
  return false;
}
