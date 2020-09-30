#pragma once

class Parser {
public:
  Parser(vector<vector<int>>& table) : table(table) {}

  void parseCode(const vector<PSToken> &tokens) {
    db(tokens);
    vector<int> stack({1});

    for (auto token : tokens) {
      db(token2String(token.primaryToken));
      int nextState;
      while ((nextState = table[stack.back()][token.primaryToken]) < 0) {
        Reduction reduction = getReductionFromId(-nextState);
        dbs("Reduction to token " + token2String(reduction.leftToken));
        for (int i = 0; i < reduction.len; i++) {
          stack.pop_back();
        }
        stack.push_back(table[stack.back()][reduction.leftToken]);
        db(stack);
      }
      stack.push_back(nextState);
      db(stack);
    }
  }
private:
  vector<vector<int>> table;
};
