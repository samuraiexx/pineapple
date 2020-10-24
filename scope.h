#pragma once

#include <vector>

#include "type.h"

namespace Scope {
  struct Object {
    int nName;
    Object* pNext;
    Type::Kind eKind;

    union {
        struct {
            Object* pType;
        } Var, Param, Field;

        struct {
            Object* pRetType;
            Object* pParams;
        } Function;

        struct {
            Object* pElemType;
            int nNumElems;
        } Array;

        struct {
            Object* pFields;
        } Struct;

        struct {
            Object* pBaseType;
        } Alias;
    } __;
  };

  int newBlock(void);
  int endBlock(void);

  Object* define(int aName);
  Object* search(int aName);
  Object* find(int aName);
};

