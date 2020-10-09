#include "parser.h"
#include "debug.h"
#include "scope.h"

using namespace std;

Parser::Parser(vector<vector<int>>& table) : table(table) {}

void Parser::parseCode(const vector<PSToken>& tokens) {
  db(tokens);

  vector<pair<int, Attrib>> stack({ {1, Attrib()} });

  int i = 0;
  db(token2String(tokens[0].primaryToken));

  do {
    PSToken token = tokens[i];
    int action = table[stack.back().first][token.primaryToken];

    if (!action) {
      dbs("Syntatic Error");
      db(token2String(token.primaryToken) _ stack);
      return;
    }
    if (isReduction(action)) {
      Reduction reduction = getReductionFromId(-action);
      // dbs("Reduction to token " + token2String(reduction.leftToken));

      vector<Attrib> args;
      for (int i = 0; i < reduction.len; i++) {
        args.push_back(stack.back().second);
        stack.pop_back();
      }
      auto nextAttrib = processSemantics(reduction, args);

      stack.push_back({ table[stack.back().first][reduction.leftToken], nextAttrib });
    }
    else {
      Attrib attrib;
      attrib.secondaryToken = token.secondaryToken;

      stack.push_back({ action, attrib });
      i++;
      // db(token2String(token.primaryToken));
    }
    // db(stack);
  } while (i < tokens.size());
}

Attrib Parser::processSemantics(Reduction reduction, vector<Attrib> args) {
  Attrib attrib;
  // db(reduction _ args);

  switch (getReductionId(reduction)) {
  case staticGetReductionId(NB, 0):
    Scope::newBlock();
    break;
  case staticGetReductionId(DT, 1):
    Scope::endBlock();
    break;
  case staticGetReductionId(DF, 0):
    Scope::endBlock();
    break;
  case staticGetReductionId(IDD, 0):
    attrib = idDefinition(args);
    break;
  case staticGetReductionId(IDU, 0):
    attrib = idUsage(args);
    break;
  case staticGetReductionId(_ID, 0):
    attrib = dotId(args);
    break;
  default:
    dbs("Reduction function not implemented: " _ reduction _ args);
    break;
  }

  return attrib;
}

inline bool Parser::isReduction(int action) {
  return action < 0;
}

Attrib Parser::idDefinition(const vector<Attrib>& args) {
  const int name = args[0].secondaryToken;
  Attrib IDD;
  IDD.secondaryToken = name;
  Scope::Object* obj = nullptr;

  if ((obj = Scope::search(name)) != nullptr) {
    dbs("ERR_REDECL");
  }
  else {
    obj = Scope::define(name);
  }
  IDD.obj = obj;

  return IDD;
}

Attrib Parser::idUsage(const std::vector<Attrib>& args) {
  const int name = args[0].secondaryToken;
  Attrib IDU;
  Scope::Object* obj = nullptr;
  IDU.secondaryToken = name;

  if ((obj = Scope::find(name)) == nullptr) {
    dbs("ERR_NOT_DECL");
    obj = Scope::define(name);
  }

  IDU.obj = obj;
  return IDU;
}

Attrib Parser::dotId(const std::vector<Attrib>& args) {
  const int name = args[0].secondaryToken;
  Attrib IDU;
  IDU.secondaryToken = name;
  IDU.obj = NULL;

  return IDU;
}
