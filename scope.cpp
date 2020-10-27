#include"scope.h"
#include "debug.h"

const int MAX_NEST_LEVEL = 100;

Scope::Object* SymbolTable[MAX_NEST_LEVEL];
Scope::Object* SymbolTableLast[MAX_NEST_LEVEL];
int nCurrentLevel = 0;

namespace Scope {
  int newBlock() {
    SymbolTable[++nCurrentLevel] = nullptr;
    SymbolTableLast[nCurrentLevel] = nullptr;
    return nCurrentLevel;
  }

  int endBlock() {
    return --nCurrentLevel;
  }

  Object* define(int aName) {
    // db(SymbolTable[nCurrentLevel]);
    Object* obj = new Object;
    obj->nName = aName;
    obj->pNext = nullptr;
    if (SymbolTable[nCurrentLevel] == nullptr) {
      SymbolTable[nCurrentLevel] = obj;
      SymbolTableLast[nCurrentLevel] = obj;
    } else {
      SymbolTableLast[nCurrentLevel]->pNext = obj;
      SymbolTableLast[nCurrentLevel] = obj;
    }
    // db(SymbolTable[nCurrentLevel]);
    return obj;
  }

  Object* search(int aName) {
    Object* obj = SymbolTable[nCurrentLevel];
    while (obj != nullptr) {
      if (obj->nName == aName) {
        break;
      } else {
        obj = obj->pNext;
      }
    }
    return obj;
  }

  Object* find(int aName) {
    Object *obj = nullptr;
    for (int i = nCurrentLevel; i >= 0 && obj == nullptr; --i) {
      obj = SymbolTable[i];
      while (obj != nullptr) {
        if (obj->nName == aName) {
          break;
        } else {
          obj = obj->pNext;
        }
      }
      if (obj != nullptr) {
        break;
      }
    }
    return obj;
  }
};

