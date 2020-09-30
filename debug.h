#define db(x) cout << #x << " = " << x << endl
#define _ << ", " << 

#include <iostream>
#include <vector>
#include <string>

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
    os << token2String(v[i]);
    if (i != v.size() - 1)
      os << ", ";
  }
  os << "]";
  return os;
}
