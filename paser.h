#pragma once

struct Attrib {
  Attrib() {}
  Attrib(PSToken token) {}
};

class Parser {
public:
  Parser(vector<vector<int>>& table) : table(table) {}

  void parseCode(const vector<PSToken> &tokens) {
    // db(tokens);

    vector<pair<int, Attrib>> stack({ {1, Attrib()} });

    int i = 0;
    // db(token2String(tokens[0].primaryToken));

    do {
      PSToken token = tokens[i];
      int action = table[stack.back().first][token.primaryToken];

      if (!action) {
        dbs("Syntatic Error");
        db(token2String(token.primaryToken) _ stack);
        return;
      }
      if(isReduction(action)) {
        Reduction reduction = getReductionFromId(-action);
        // dbs("Reduction to token " + token2String(reduction.leftToken));

        vector<Attrib> args;
        for (int i = 0; i < reduction.len; i++) {
          args.push_back(stack.back().second);
          stack.pop_back();
        }
        auto nextAttrib = processSemantics(reduction, args);

        stack.push_back({ table[stack.back().first][reduction.leftToken], nextAttrib });
      } else {
        stack.push_back({ action, Attrib(token) });
        i++;
        // db(token2String(token.primaryToken));
      }
      // db(stack);
    } while (i < tokens.size());
  }
private:
  vector<vector<int>> table;

  Attrib processSemantics(Reduction reduction, vector<Attrib> args) {
    return args[0];
  }

  inline bool isReduction(int action) {
    return action < 0;
  }
};

std::ostream& operator<<(std::ostream& os, const Attrib &attrib) {
  return os;
}
