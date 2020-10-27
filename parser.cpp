#include "parser.h"
#include "debug.h"
#include "scope.h"
#include "type.h"
#include "builtin.h"

using namespace std;
using Kind = Type::Kind;

Attrib TopSem(int x) {
  // TODO: implement this one
  Attrib ret;
  return ret;
}

bool checkTypes(Scope::Object* t1, Scope::Object* t2) {
  if (t1 == t2) { return true; }
  else if (t1 == BuiltIn::pUniversal || t2 == BuiltIn::pUniversal) {
    return true;
  }
  else if (t1->eKind == Kind::UNIVERSAL || t2->eKind == Kind::UNIVERSAL) {
    return true;
  }
  else if (t1->eKind == t2->eKind) {
    if (t1->eKind == Kind::ALIAS_TYPE) {
      return checkTypes(t1->__.Alias.pBaseType, t2->__.Alias.pBaseType);
    }
    else if (t1->eKind == Kind::ALIAS_TYPE) {
      if (t1->__.Array.nNumElems == t2->__.Array.nNumElems) {
        return checkTypes(t1->__.Array.pElemType,
                          t2->__.Array.pElemType);
      }
    }
    else if (t1->eKind == Kind::STRUCT_TYPE) {
      Scope::Object* f1 = t1->__.Struct.pFields;
      Scope::Object* f2 = t2->__.Struct.pFields;

      while (f1 != NULL && f2 != NULL) {
        if (!checkTypes(f1->__.Field.pType, f2->__.Field.pType)) {
          return false;
        }
      }
      return (f1 == NULL && f2 == NULL);
    }
  }
  return false;
}

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
      reverse(args.begin(), args.end());
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
  db(reduction _ args);
  
  switch (getReductionId(reduction)) {
  case staticGetReductionId(T, 0):
    attrib = integerType();
    break;
  case staticGetReductionId(T, 1):
    attrib = charType();
    break;
  case staticGetReductionId(T, 2):
    attrib = booleanType();
    break;
  case staticGetReductionId(T, 3):
    attrib = stringType();
    break;
  case staticGetReductionId(T, 4):
    attrib = declaredType(args);
    break;
  case staticGetReductionId(DT, 0):
    attrib = arrayDeclaration(args);
    break;
  case staticGetReductionId(DT, 1):
    attrib = structDeclaration(args);
    break;
  case staticGetReductionId(DT, 2):
    attrib = aliasDeclaration(args);
    break;
  case staticGetReductionId(DC, 0):
    attrib = fieldList(args);
    break;
  case staticGetReductionId(DC, 1):
    attrib = fieldListElement(args);
    break;
  case staticGetReductionId(DF, 0):
    Scope::endBlock();
    break;
  case staticGetReductionId(LP, 0):
    attrib = parameterList(args);
    break;
  case staticGetReductionId(LP, 1):
    attrib = parameterListElement(args);
    break;
  case staticGetReductionId(DV, 0):
    attrib = variableDeclaration(args);
    break;
  case staticGetReductionId(LI, 0):
    attrib = identifierListElement(args);
    break;
  case staticGetReductionId(LI, 1):
    attrib = identifierList(args);
    break;
  case staticGetReductionId(S, 0):
    attrib = ifStatement(args);
    break;
  case staticGetReductionId(S, 1):
    attrib = ifElseStatement(args);
    break;
  case staticGetReductionId(S, 2):
    attrib = whileStatement(args);
    break;
  case staticGetReductionId(S, 3):
    attrib = doWhileStatement(args);
    break;
  case staticGetReductionId(S, 4):
    Scope::newBlock();
    break;
  case staticGetReductionId(S, 5):
    attrib = assignment(args);
    break;
  case staticGetReductionId(E, 0):
    attrib = logicOperationAnd(args);
    break;
  case staticGetReductionId(E, 1):
    attrib = logicOperationOr(args);
    break;
  case staticGetReductionId(E, 2):
    attrib = uniqueLogicOperand(args);
    break;
  case staticGetReductionId(L, 0):
    attrib = lessComparation(args);
    break;
  case staticGetReductionId(L, 1):
    attrib = greaterComparation(args);
    break;
  case staticGetReductionId(L, 2):
    attrib = lessOrEqualComparation(args);
    break;
  case staticGetReductionId(L, 3):
    attrib = greaterOrEqualComparation(args);
    break;
  case staticGetReductionId(L, 4):
    attrib = equalComparation(args);
    break;
  case staticGetReductionId(L, 5):
    attrib = notEqualComparation(args);
    break;
  case staticGetReductionId(L, 6):
    attrib = rValueComparation(args);
    break;
  case staticGetReductionId(R, 0):
    attrib = arithmeticSum(args);
    break;
  case staticGetReductionId(R, 1):
    attrib = arithmeticSub(args);
    break;
  case staticGetReductionId(R, 2):
    attrib = arithmeticRY(args);
    break;
  case staticGetReductionId(Y, 0):
    attrib = arithmeticMul(args);
    break;
  case staticGetReductionId(Y, 1):
    attrib = arithmeticDiv(args);
    break;
  case staticGetReductionId(Y, 2):
    attrib = arithmeticYF(args);
    break;
  case staticGetReductionId(F, 0):
    attrib = unaryFLV(args);
    break;
  case staticGetReductionId(F, 1):
    attrib = unaryLPlusPlus(args);
    break;
  case staticGetReductionId(F, 2):
    attrib = unaryLMinusMinus(args);
    break;
  case staticGetReductionId(F, 3):
    attrib = unaryRPlusPlus(args);
    break;
  case staticGetReductionId(F, 4):
    attrib = unaryRMinusMinus(args);
    break;
  case staticGetReductionId(F, 5):
    attrib = unaryParenthesis(args);
    break;
  case staticGetReductionId(F, 6):
    attrib = functionCall(args);
    break;
  case staticGetReductionId(F, 7):
    attrib = unaryMinus(args);
    break;
  case staticGetReductionId(F, 8):
    attrib = unaryNot(args);
    break;
  case staticGetReductionId(F, 9):
    attrib = unaryTrue();
    break;
  case staticGetReductionId(F, 10):
    attrib = unaryFalse();
    break;
  case staticGetReductionId(F, 11):
    attrib = unaryChr();
    break;
  case staticGetReductionId(F, 12):
    attrib = unaryStr();
    break;
  case staticGetReductionId(F, 13):
    attrib = unaryNum();
    break;
  case staticGetReductionId(LE, 0):
    attrib = callArgumentList(args);
    break;
  case staticGetReductionId(LE, 1):
    attrib = callArgumentListElement(args);
    break;
  case staticGetReductionId(LV, 0):
    attrib = structFieldAccess(args);
    break;
  case staticGetReductionId(LV, 1):
    attrib = arrayIndexAccess(args);
    break;
  case staticGetReductionId(LV, 2):
    attrib = lvIDU(args);
  case staticGetReductionId(NB, 0):
    Scope::newBlock();
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
  case staticGetReductionId(_TRUE, 0):
    attrib = boolValue();
    break;
  case staticGetReductionId(_FALSE, 0):
    attrib = boolValue();
    break;
  case staticGetReductionId(_CHARACTER, 0):
    attrib = charValue(args);
    break;
  case staticGetReductionId(_STRINGVAL, 0):
    attrib = stringValue(args);
    break;
  case staticGetReductionId(_NUMERAL, 0):
    attrib = numValue(args);
    break;
  case staticGetReductionId(MF, 0):
    attrib = functionMarker(args);
    break;
  case staticGetReductionId(MC, 0):
    attrib = callMarker(args);
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

Attrib Parser::integerType() {
  Attrib T;
  T.type = BuiltIn::pInt;
  return T;
}

Attrib Parser::charType() {
  Attrib T;
  T.type = BuiltIn::pChar;
  return T;
}

Attrib Parser::booleanType() {
  Attrib T;
  T.type = BuiltIn::pBool;
  return T;
}

Attrib Parser::stringType() {
  Attrib T;
  T.type = BuiltIn::pString;
  return T;
}

Attrib Parser::declaredType(const vector<Attrib>& args) {
  const Attrib IDU = args[0];
  Scope::Object* p = IDU.obj;

  Attrib T;

  if (Type::IS_TYPE_KIND(p->eKind) || p->eKind == Kind::UNIVERSAL) {
    T.type = p;
  }
  else {
    T.type = BuiltIn::pUniversal;
    dbs("ERR_TYPE_EXPECTED");
  }
  return T;
}

Attrib Parser::arrayDeclaration(const vector<Attrib>& args) {
  Attrib IDD = args[1];
  Attrib NUM = args[5];
  Attrib T = args[8];

  Scope::Object* p = IDD.obj;
  int n = NUM.val;
  Scope::Object* t = T.type;
  p->eKind = Kind::ARRAY_TYPE;
  p->__.Array.nNumElems = n;
  p->__.Array.pElemType = t;

  Attrib DT;
  return DT;
}

Attrib Parser::structDeclaration(const vector<Attrib>& args) {
  Attrib IDD = args[1];
  Attrib DC = args[6];

  Scope::Object* p = IDD.obj;

  p->eKind = Kind::STRUCT_TYPE;
  p->__.Struct.pFields = DC.list;

  Scope::endBlock();

  Attrib DT;
  return DT;
}

Attrib Parser::aliasDeclaration(const vector<Attrib>& args) {
  Attrib IDD = args[1];
  Attrib T = args[3];

  Scope::Object* p = IDD.obj;
  Scope::Object* t = T.type;

  p->eKind = Kind::ALIAS_TYPE;
  p->__.Alias.pBaseType = t;

  Attrib DT;
  return DT;
}

Attrib Parser::fieldList(const vector<Attrib>& args) {
  Attrib DC1 = args[0];
  Attrib LI = args[2];
  Attrib T = args[4];

  Scope::Object* p = LI.list;
  Scope::Object* t = T.type;

  while (p != NULL && p->eKind == Kind::NO_KIND_DEF) {
    p->eKind = Kind::FIELD;
    p->__.Field.pType = t;
    p = p->pNext;
  }

  Attrib DC0;
  DC0.list = DC1.list;
  return DC0;
}

Attrib Parser::fieldListElement(const vector<Attrib>& args) {
  Attrib LI = args[0];
  Attrib T = args[2];

  Scope::Object* p = LI.obj;
  Scope::Object* t = T.type;

  while (p != NULL && p->eKind == Kind::NO_KIND_DEF) {
    p->eKind = Kind::FIELD;
    p->__.Field.pType = t;
    p = p->pNext;
  }

  Attrib DC;
  DC.list = LI.list;
  return DC;
}

Attrib Parser::parameterList(const vector<Attrib>& args) {
  Attrib LP1 = args[0];
  Attrib IDD = args[2];
  Attrib T = args[4];

  Scope::Object* p = IDD.obj;
  Scope::Object* t = T.type;

  p->eKind = Kind::PARAM;
  p->__.Param.pType = t;

  Attrib LP0;
  LP0.list = LP1.list;
  return LP0;
}

Attrib Parser::parameterListElement(const vector<Attrib>& args) {
  Attrib IDD = args[0];
  Attrib T = args[2];

  Scope::Object* p = IDD.obj;
  Scope::Object* t = T.type;

  p->eKind = Kind::PARAM;
  p->__.Param.pType = t;

  Attrib LP;
  LP.list = p;
  return LP;
}

Attrib Parser::variableDeclaration(const vector<Attrib>& args) {
  const Attrib LI = args[1];
  const Attrib T = args[3];

  Scope::Object* p = LI.list;
  Scope::Object* t = T.type;

  while (p != NULL && p->eKind == Kind::NO_KIND_DEF) {
    p->eKind = Kind::VAR;
    p->__.Var.pType = t;
    p = p->pNext;
  }
  
  Attrib DV;
  return DV;
}

Attrib Parser::identifierListElement(const vector<Attrib>& args) {
  const Attrib IDD = args[0];
  Attrib LI;

  LI.list = IDD.obj;
  return LI;
}

Attrib Parser::identifierList(const vector<Attrib>& args) {
  const Attrib LI1 = args[0];
  Attrib LI0;

  LI0.list = LI1.list;
  return LI0;
}

Attrib Parser::ifStatement(const vector<Attrib>& args) {
  const Attrib E = args[2];
  Scope::Object* t = E.type;
  if (!checkTypes(t, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }
  Attrib S;
  return S;
}

Attrib Parser::ifElseStatement(const vector<Attrib>& args) {
  const Attrib E = args[2];
  Scope::Object* t = E.type;
  if (!checkTypes(t, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }
  Attrib S;
  return S;
}

Attrib Parser::whileStatement(const vector<Attrib>& args) {
  const Attrib E = args[2];
  Scope::Object* t = E.type;
  if (!checkTypes(t, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }
  Attrib S;
  return S;
}

Attrib Parser::doWhileStatement(const vector<Attrib>& args) {
  const Attrib E = args[4];
  Scope::Object* t = E.type;
  if (!checkTypes(t, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }
  Attrib S;
  return S;
}

Attrib Parser::assignment(const vector<Attrib>& args) {
  const Attrib LV = args[0];
  const Attrib E = args[2];
  Scope::Object* tLV = LV.type;
  Scope::Object* tE = E.type;
  if(!checkTypes(tLV, tE)) {
    dbs("ERR_TYPE_MISMATCH");
  }
  Attrib S;
  return S;
}

Attrib Parser::logicOperationAnd(const vector<Attrib>& args) {
  const Attrib E = args[0];
  const Attrib L = args[2];

  if(!checkTypes(E.type, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }

  if(!checkTypes(L.type, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }

  Attrib E0;
  E0.type = BuiltIn::pBool;
  return E0;
}

Attrib Parser::logicOperationOr(const vector<Attrib>& args) {
  const Attrib E = args[0];
  const Attrib L = args[2];

  if(!checkTypes(E.type, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }

  if(!checkTypes(L.type, BuiltIn::pBool)) {
    dbs("ERR_BOOL_TYPE_EXPECTED");
  }

  Attrib E0;
  E0.type = BuiltIn::pBool;
  return E0;
}

Attrib Parser::uniqueLogicOperand(const vector<Attrib>& args) {
  const Attrib L = args[0];

  Attrib E;
  E.type = L.type;
  return E;
}

Attrib Parser::lessComparation(const vector<Attrib>& args) {
  const Attrib L = args[0];
  const Attrib R = args[2];

  if (!checkTypes(L.type, R.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }

  Attrib L0;
  L0.type = BuiltIn::pBool;
  return L0;
}

Attrib Parser::greaterComparation(const vector<Attrib>& args) {
  const Attrib L = args[0];
  const Attrib R = args[2];

  if (!checkTypes(L.type, R.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }

  Attrib L0;
  L0.type = BuiltIn::pBool;
  return L0;
}

Attrib Parser::lessOrEqualComparation(const vector<Attrib>& args) {
  const Attrib L = args[0];
  const Attrib R = args[2];

  if (!checkTypes(L.type, R.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }

  Attrib L0;
  L0.type = BuiltIn::pBool;
  return L0;
}

Attrib Parser::greaterOrEqualComparation(const vector<Attrib>& args) {
  const Attrib L = args[0];
  const Attrib R = args[2];

  if (!checkTypes(L.type, R.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }

  Attrib L0;
  L0.type = BuiltIn::pBool;
  return L0;
}

Attrib Parser::equalComparation(const vector<Attrib>& args) {
  const Attrib L = args[0];
  const Attrib R = args[2];

  if (!checkTypes(L.type, R.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }

  Attrib L0;
  L0.type = BuiltIn::pBool;
  return L0;
}

Attrib Parser::notEqualComparation(const vector<Attrib>& args) {
  const Attrib L = args[0];
  const Attrib R = args[2];

  if (!checkTypes(L.type, R.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }

  Attrib L0;
  L0.type = BuiltIn::pBool;
  return L0;
}

Attrib Parser::rValueComparation(const vector<Attrib>& args) {
  const Attrib R = args[0];

  Attrib L;
  L.type = R.type;
  return L;
}

Attrib Parser::arithmeticSum(const vector<Attrib>& args) {
  const Attrib R = args[0];
  const Attrib Y = args[2];

  if (!checkTypes(R.type, Y.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }
  if (!checkTypes(R.type, BuiltIn::pInt) &&
      !checkTypes(R.type, BuiltIn::pString)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib R0;
  R0.type = R.type;
  return R0;
}

Attrib Parser::arithmeticSub(const vector<Attrib>& args) {
  const Attrib R = args[0];
  const Attrib Y = args[2];

  if (!checkTypes(R.type, Y.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }
  if (!checkTypes(R.type, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib R0;
  R0.type = R.type;
  return R0;
}

Attrib Parser::arithmeticRY(const vector<Attrib>& args) {
  const Attrib Y = args[0];

  Attrib R;
  R.type = Y.type;
  return R;
}

Attrib Parser::arithmeticMul(const vector<Attrib>& args) {
  const Attrib Y = args[0];
  const Attrib F = args[2];

  if (!checkTypes(Y.type, F.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }
  if (!checkTypes(Y.type, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib Y0;
  Y0.type = Y.type;
  return Y0;
}

Attrib Parser::arithmeticDiv(const vector<Attrib>& args) {
  const Attrib Y = args[0];
  const Attrib F = args[2];

  if (!checkTypes(Y.type, F.type)) {
    dbs("ERR_TYPE_MISMATCH");
  }
  if (!checkTypes(Y.type, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib Y0;
  Y0.type = Y.type;
  return Y0;
}

Attrib Parser::arithmeticYF(const vector<Attrib>& args) {
  const Attrib F = args[0];

  Attrib Y;
  Y.type = F.type;
  return Y;
}

Attrib Parser::unaryFLV(const vector<Attrib>& args) {
  const Attrib LV = args[0];

  Attrib F;
  F.type = LV.type;
  return F;
}

Attrib Parser::unaryLPlusPlus(const vector<Attrib>& args) {
  const Attrib LV = args[1];
  Scope::Object* t = LV.type;

  if(!checkTypes(t, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib F;
  F.type = BuiltIn::pInt;
  return F;
}

Attrib Parser::unaryLMinusMinus(const vector<Attrib>& args) {
  const Attrib LV = args[1];
  Scope::Object* t = LV.type;

  if(!checkTypes(t, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib F;
  F.type = BuiltIn::pInt;
  return F;
}

Attrib Parser::unaryRPlusPlus(const vector<Attrib>& args) {
  const Attrib LV = args[0];
  Scope::Object* t = LV.type;

  if(!checkTypes(t, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib F;
  F.type = BuiltIn::pInt;
  return F;
}

Attrib Parser::unaryRMinusMinus(const vector<Attrib>& args) {
  const Attrib LV = args[0];
  Scope::Object* t = LV.type;

  if(!checkTypes(t, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib F;
  F.type = BuiltIn::pInt;
  return F;
}

Attrib Parser::unaryParenthesis(const vector<Attrib>& args) {
  const Attrib E = args[1];

  Attrib F;
  F.type = E.type;
  return F;
}

Attrib Parser::functionCall(const vector<Attrib>& args) {
  const Attrib IDU = args[0];
  const Attrib MC = args[1];
  const Attrib LE = args[3];
  Attrib F;
  F.type = MC.type;

  if (!MC.err) {
    if (LE.param != NULL) {
      dbs("ERR_TOO_FEW_ARGS");
    }
  }

  return F;
}

Attrib Parser::unaryMinus(const vector<Attrib>& args) {
  const Attrib LV = args[1];
  Scope::Object* t = LV.type;

  if(!checkTypes(t, BuiltIn::pInt)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib F;
  F.type = BuiltIn::pInt;
  return F;
}

Attrib Parser::unaryNot(const vector<Attrib>& args) {
  const Attrib LV = args[1];
  Scope::Object* t = LV.type;

  if(!checkTypes(t, BuiltIn::pBool)) {
    dbs("ERR_INVALID_TYPE");
  }

  Attrib F;
  F.type = BuiltIn::pBool;
  return F;
}

Attrib Parser::unaryTrue() {
  Attrib F;
  F.type = BuiltIn::pBool;
  return F;
}

Attrib Parser::unaryFalse() {
  Attrib F;
  F.type = BuiltIn::pBool;
  return F;
}

Attrib Parser::unaryChr() {
  Attrib F;
  F.type = BuiltIn::pChar;
  return F;
}

Attrib Parser::unaryStr() {
  Attrib F;
  F.type = BuiltIn::pString;
  return F;
}

Attrib Parser::unaryNum() {
  Attrib F;
  F.type = BuiltIn::pInt;
  return F;
}

Attrib Parser::callArgumentList(const vector<Attrib>& args) {
  const Attrib LE = args[0];
  const Attrib E = args[2];
  Attrib LE0;
  LE0.param = NULL;
  LE0.err = LE.err;
  int n = LE.n;

  if (!LE.err) {
    Scope::Object* p = LE.param;
    if (p == NULL) {
      dbs("ERR_TOO_MANY_ARGS");
      LE0.err = true;
    }
    else {
      if (!checkTypes(p->__.Param.pType, E.type)) {
        dbs("ERR_PARAM_TYPE_N");
      }
      LE0.param = p->pNext;
      LE0.n = n+1;
    }
  }

  return LE0;
}

Attrib Parser::callArgumentListElement(const vector<Attrib>& args) {
  Attrib E = TopSem(0);
  Attrib MC = TopSem(-1);

  Attrib LE;
  LE.param = NULL;
  LE.err = MC.err;
  int n = 1;
  if (!MC.err) {
    Scope::Object* p = MC.param;
    if (p == NULL) {
      dbs("ERR_TOO_MANY_ARGS");
      LE.err = true;
    }
    else {
      if (!checkTypes(p->__.Param.pType, E.type)) {
        dbs("ERR_PARAM_TYPE_N");
      }
      LE.param = p->pNext;
      LE.n = n+1;
    }
  }
  return LE;
}

Attrib Parser::structFieldAccess(const vector<Attrib>& args) {
  const Attrib LV = args[0];
  const Attrib ID = args[2];
  Scope::Object* t = LV.type;

  Attrib LV0;
  if (t->eKind != Kind::STRUCT_TYPE) {
    if (t->eKind != Kind::UNIVERSAL) {
      dbs("ERR_KIND_NOT_STRUCT");
    }
    LV0.type = BuiltIn::pUniversal;
  }
  else {
    Scope::Object* p = t->__.Struct.pFields;
    while (p != NULL) {
      if (p->nName == ID.name) {
        break;
      }
      p = p->pNext;
    }

    if (p == NULL) {
      dbs("ERR_FIELD_NOT_DECL");
      LV0.type = BuiltIn::pUniversal;
    }
    else {
      LV0.type = p->__.Field.pType;
    }
  }

  return LV0;
}

Attrib Parser::arrayIndexAccess(const vector<Attrib>& args) {
  const Attrib LV = args[0];
  const Attrib E = args[2];
  Scope::Object* t = LV.type;

  Attrib LV0;
  if (t == BuiltIn::pString) {
    LV0.type = BuiltIn::pChar;
  }
  else if (t->eKind != Kind::ARRAY_TYPE) {
    if (t->eKind != Kind::UNIVERSAL) {
      dbs("ERR_KIND_NOT_ARRAY");
    }
    LV0.type = BuiltIn::pUniversal;
  }
  else {
    LV0.type = t->__.Array.pElemType;
  }

  if(!checkTypes(E.type, BuiltIn::pInt)) {
    dbs("ERR_INVALID_INDEX_TYPE");
  }

  return LV0;
}

Attrib Parser::lvIDU(const vector<Attrib>& args) {
  const Attrib IDU = args[0];
  Scope::Object* p = IDU.obj;

  Attrib LV;
  if (p->eKind != Kind::VAR &&
      p->eKind != Kind::PARAM) {
    if (p->eKind != Kind::UNIVERSAL) {
      dbs("ERR_KIND_NOT_VAR");
    }
    LV.type = BuiltIn::pUniversal;
  }
  else {
    LV.type = p->__.Var.pType;
  }
  return LV;
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
  obj->eKind = Kind::NO_KIND_DEF;
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

Attrib Parser::boolValue() {
  Attrib BOOL;
  BOOL.type = BuiltIn::pBool;
  return BOOL;
}

Attrib Parser::charValue(const std::vector<Attrib>& args) {
  const int p = args[0].secondaryToken;
  Attrib CHR;
  CHR.type = BuiltIn::pChar;
  CHR.pos = p;
  return CHR;
}

Attrib Parser::stringValue(const std::vector<Attrib>& args) {
  const int p = args[0].secondaryToken;
  Attrib STR;
  STR.type = BuiltIn::pString;
  STR.pos = p;
  return STR;
}

Attrib Parser::numValue(const std::vector<Attrib>& args) {
  const int p = args[0].secondaryToken;
  Attrib NUM;
  NUM.type = BuiltIn::pInt; 
  NUM.pos = p;
  NUM.val = std::as_const(NUM.pos);
  return NUM;
}

Attrib Parser::functionMarker(const std::vector<Attrib>& args) {
  Attrib T = TopSem(0);
  Attrib LP = TopSem(-1);
  Attrib NB = TopSem(-2);
  Attrib IDD = TopSem(-3);

  dbs("calling function marker");

  Scope::Object* f = IDD.obj;
  f->eKind = Kind::FUNCTION;
  f->__.Function.pRetType = T.type;
  f->__.Function.pParams = LP.list;

  Attrib MF;
  return MF;
}

Attrib Parser::callMarker(const std::vector<Attrib>& args) {
  Attrib IDU = TopSem(0);
  Scope::Object* f = IDU.obj;

  Attrib MC;
  if (f->eKind != Kind::FUNCTION) {
    dbs("ERR_KIND_NOT_FUNC");
    MC.type = BuiltIn::pUniversal;
    MC.param = NULL;
    MC.err = true;
  }
  else {
    MC.type = f->__.Function.pRetType;
    MC.param = f->__.Function.pParams;
    MC.err = false;
  }
  return MC;
}