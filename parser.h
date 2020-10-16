#pragma once

#include <vector>
#include <utility>

#include "tokenizer.h"
#include "rules.h"
#include "scope.h"
#include "type.h"

struct Attrib {
  int secondaryToken;
  Scope::Object* obj;

  Attrib() : secondaryToken(-1), obj(nullptr) {}
};

class Parser {
public:
  Parser(std::vector<std::vector<int>>& table);
  void parseCode(const std::vector<PSToken>& tokens);

private:
  std::vector<std::vector<int>> table;
  Attrib processSemantics(Reduction reduction, std::vector<Attrib> args);
  inline bool isReduction(int action);

  Attrib idDefinition(const std::vector<Attrib> &args);
  Attrib idUsage(const std::vector<Attrib> &args);
  Attrib dotId(const std::vector<Attrib> &args);
};
