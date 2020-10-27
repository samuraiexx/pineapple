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
  Scope::Object* type;
  Scope::Object* list;
  Scope::Object* param;
  int pos;
  int val;
  int name;
  bool err;
  int n;

  Attrib() : secondaryToken(-1), obj(nullptr), type(nullptr), list(nullptr), param(nullptr), pos(0), val(0), name(0), err(false), n(0) {}
};

class Parser {
public:
  Parser(std::vector<std::vector<int>>& table);
  void parseCode(const std::vector<PSToken>& tokens);

private:
  std::vector<std::vector<int>> table;
  Attrib processSemantics(Reduction reduction, std::vector<Attrib> args);
  inline bool isReduction(int action);

  Attrib integerType();
  Attrib charType();
  Attrib booleanType();
  Attrib stringType();
  Attrib declaredType(const vector<Attrib>& args);
  Attrib arrayDeclaration(const vector<Attrib>& args);
  Attrib structDeclaration(const vector<Attrib>& args);
  Attrib aliasDeclaration(const vector<Attrib>& args);
  Attrib fieldList(const vector<Attrib>& args);
  Attrib fieldListElement(const vector<Attrib>& args);
  Attrib parameterList(const vector<Attrib>& args);
  Attrib parameterListElement(const vector<Attrib>& args);
  Attrib variableDeclaration(const vector<Attrib>& args);
  Attrib identifierListElement(const vector<Attrib>& args);
  Attrib identifierList(const vector<Attrib>& args);
  Attrib ifStatement(const vector<Attrib>& args);
  Attrib ifElseStatement(const vector<Attrib>& args);
  Attrib whileStatement(const vector<Attrib>& args);
  Attrib doWhileStatement(const vector<Attrib>& args);
  Attrib assignment(const vector<Attrib>& args);
  Attrib logicOperationAnd(const vector<Attrib>& args);
  Attrib logicOperationOr(const vector<Attrib>& args);
  Attrib uniqueLogicOperand(const vector<Attrib>& args);
  Attrib lessComparation(const vector<Attrib>& args);
  Attrib greaterComparation(const vector<Attrib>& args);
  Attrib lessOrEqualComparation(const vector<Attrib>& args);
  Attrib greaterOrEqualComparation(const vector<Attrib>& args);
  Attrib equalComparation(const vector<Attrib>& args);
  Attrib notEqualComparation(const vector<Attrib>& args);
  Attrib rValueComparation(const vector<Attrib>& args);
  Attrib arithmeticSum(const vector<Attrib>& args);
  Attrib arithmeticSub(const vector<Attrib>& args);
  Attrib arithmeticRY(const vector<Attrib>& args);
  Attrib arithmeticMul(const vector<Attrib>& args);
  Attrib arithmeticDiv(const vector<Attrib>& args);
  Attrib arithmeticYF(const vector<Attrib>& args);
  Attrib unaryFLV(const vector<Attrib>& args);
  Attrib unaryLPlusPlus(const vector<Attrib>& args);
  Attrib unaryLMinusMinus(const vector<Attrib>& args);
  Attrib unaryRPlusPlus(const vector<Attrib>& args);
  Attrib unaryRMinusMinus(const vector<Attrib>& args);
  Attrib unaryParenthesis(const vector<Attrib>& args);
  Attrib functionCall(const vector<Attrib>& args);
  Attrib unaryMinus(const vector<Attrib>& args);
  Attrib unaryNot(const vector<Attrib>& args);
  Attrib unaryTrue();
  Attrib unaryFalse();
  Attrib unaryChr();
  Attrib unaryStr();
  Attrib unaryNum();
  Attrib callArgumentList(const vector<Attrib>& args);
  Attrib callArgumentListElement(const vector<Attrib>& args);
  Attrib structFieldAccess(const std::vector<Attrib>& args);
  Attrib arrayIndexAccess(const std::vector<Attrib>& args);
  Attrib lvIDU(const std::vector<Attrib>& args);
  Attrib idDefinition(const std::vector<Attrib>& args);
  Attrib idUsage(const std::vector<Attrib>& args);
  Attrib dotId(const std::vector<Attrib>& args);
  Attrib boolValue();
  Attrib charValue(const std::vector<Attrib> &args);
  Attrib stringValue(const std::vector<Attrib>& args);
  Attrib numValue(const std::vector<Attrib>& args);
  Attrib functionMarker(const std::vector<Attrib>& args);
  Attrib callMarker(const std::vector<Attrib>& args);
};
