#pragma once
#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

#define db(x) cout << #x << " = " << x << endl
#define dbs(x) cout << x << endl
#define _ << ", " << 

#include "parser.h"
#include "rules.h"
#include "token.h"

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v);
std::ostream& operator<<(std::ostream& os, const Reduction& r);
std::ostream& putTable(std::ostream& os, const std::vector<std::vector<int>>& table, bool _skipColumn = true);
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<int>>& table);
std::ostream& operator<<(std::ostream& os, const PSToken& v);
std::ostream& operator<<(std::ostream& os, const Attrib& a);
std::ostream& operator<<(std::ostream& os, const Scope::Object* obj);

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const std::pair<T, V>& v) {
  os << "{" << v.first << ", " << v.second << "}";
  return os;
}

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

template <typename T, typename V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<T, V>& v) {
  os << "[ ";
  for (auto p : v) {
    os << "{" << p.first << ", " << p.second << "}, ";
  }
  os << "]";
  return os;
}
