#pragma once

struct Attrib {
  Attrib() {}
  Attrib(PSToken token) {}
};

class Parser {
public:
  Parser(vector<vector<int>>& table) : table(table) {}

  void parseCode(const vector<PSToken> &tokens) {
    db(tokens);

    vector<pair<int, Attrib>> stack({ {1, Attrib()} });

    for (auto token : tokens) {
      int nextState;
      db(token2String(token.primaryToken));

      while ((nextState = table[stack.back().first][token.primaryToken]) < 0) {
        Reduction reduction = getReductionFromId(-nextState);
        dbs("Reduction to token " + token2String(reduction.leftToken));

        vector<Attrib> args;
        for (int i = 0; i < reduction.len; i++) {
          args.push_back(stack.back().second);
          stack.pop_back();
        }
        auto nextAttrib = processSemantics(reduction, args);

        stack.push_back({ table[stack.back().first][reduction.leftToken], nextAttrib });
        db(stack);
      }
      stack.push_back({ nextState, Attrib(token) });
      db(stack);
    }
  }
private:
  vector<vector<int>> table;

  Attrib processSemantics(Reduction reduction, vector<Attrib> args) {
    return args[0];
  }
};

std::ostream& operator<<(std::ostream& os, const Attrib &attrib) {
  return os;
}
