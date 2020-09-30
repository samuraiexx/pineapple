#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <utility>

#define db(x) cout << #x << " = " << x << endl
#define dbs(x) cout << x << endl
#define _ << ", " << 

#include "token.h"

std::string token2String(int);

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  os << "[";
  for (int i = 0; i < v.size(); ++i) {
    os << v[i];
    if (i != v.size() - 1)
      os << ", ";
  }
  os << "]";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
  os << "[";
  for (int i = 0; i < v.size(); ++i) {
    os << v[i];
    if (i != v.size() - 1)
      os << ", ";
  }
  os << "]";
  return os;
}

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<T, V>& v) {
  os << "{" << v.first << ", " << v.second << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<int>>& table) {
  std::set<int> skipColumn;

  for (int j = 0; table.size() && j < table[0].size(); j++) {
    bool onlyZeros = true;
    for (int i = 0; i < table.size(); i++) {
      onlyZeros = onlyZeros && table[i][j] == 0;
    }
    if(onlyZeros) skipColumn.insert(j);
  }

  for (int j = 0; j < table[0].size(); j++) {
    if (skipColumn.count(j)) {
      continue;
    }

    os << token2String(j) << '\t';
  }
  os << '\n';

  for (int i = 1; i < table.size(); i++) {
    for (int j = 0; j < table[0].size(); j++) {
      if (skipColumn.count(j)) {
        continue;
      }
      os << table[i][j] << '\t';
    }
    os << '\n';
  }

  return os;
}

