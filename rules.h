#pragma once
#include <unordered_map>
#include <vector>

struct Reduction {
  int len;
  int leftToken;
  int line;

  Reduction(int leftToken, int len, int line) : len(len), leftToken(leftToken), line(line) {}
};

#define staticGetReductionId(X, Y) 100 * (X) + (Y)
int getReductionId(int token, int line);
int getReductionId(Reduction reduction);
Reduction getReductionFromId(int reductionId);

extern std::unordered_map<int, std::vector<std::vector<int>>> rules;