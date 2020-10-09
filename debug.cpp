#include "debug.h"

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
  os << "[";
  for (int i = 0; i < v.size(); ++i) {
    os << token2String(v[i]);
    if (i != v.size() - 1)
      os << ", ";
  }
  os << "]";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Reduction& r) {
  os << token2String(r.leftToken) << " => " << rules[r.leftToken][r.line];
  return os;
}

std::ostream& putTable(std::ostream& os, const std::vector<std::vector<int>>& table, bool _skipColumn) {
  std::set<int> skipColumn;

  for (int j = 0; table.size() && j < table[0].size(); j++) {
    bool onlyZeros = true;
    for (int i = 0; i < table.size(); i++) {
      onlyZeros = onlyZeros && table[i][j] == 0;
    }
    if(onlyZeros) skipColumn.insert(j);
  }

  os << '\t';
  for (int j = 0; j < table[0].size(); j++) {
    if (skipColumn.count(j) && _skipColumn) {
      continue;
    }

    os << token2String(j) << "\t";
  }
  os << '\n';

  for (int i = 1; i < table.size(); i++) {
    os << i << '\t';
    for (int j = 0; j < table[0].size(); j++) {
      if (skipColumn.count(j) && _skipColumn) {
        continue;
      }
      os << table[i][j] << "\t";
    }
    os << '\n';
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<int>>& table) {
  return putTable(os, table);
}

std::ostream& operator<<(std::ostream& os, const PSToken &v) {
   os << "{" << token2String(v.primaryToken) << ", " << v.secondaryToken << "}";
  return os;
}

std::ostream& operator<<(std::ostream& os, const Attrib &a) {
  os << a.secondaryToken;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Scope::Object* obj) {
  os << "[";
  while (obj) {
    os << obj->nName << (obj->pNext ? ", " : "");
    obj = obj->pNext;
  }
  os << "]";
  return os;
}

