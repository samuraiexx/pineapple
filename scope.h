#pragma once

#include <vector>

namespace Scope {
  struct Object {
    int nName;
    Object* pNext;
  };

  int newBlock(void);
  int endBlock(void);
  Object* define(int aName);
  Object* search(int aName);
  Object* find(int aName);
};

