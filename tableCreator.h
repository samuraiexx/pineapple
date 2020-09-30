#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdarg>
#include <assert.h>

#include "token.h"
#include "rules.h"

using namespace std;
static int stateCount = 0;
struct State;
struct StateLine;
class TableCreator;

struct State {
  int id;
  vector<StateLine> stateLines;
  bool vis;

  State() {
    id = ++stateCount;
    vis = false;
  }
};

struct StateLine {
  vector<int> lookAhead;
  int genToken;
  int line;
  int statePos;

  StateLine(const StateLine &stateLine) : lookAhead(stateLine.lookAhead), genToken(stateLine.genToken), line(stateLine.line), statePos(stateLine.statePos) {}

  StateLine(int genToken, int line, vector<int> lookAhead = {}, int statePos = 0)
    : genToken(genToken), line(line), lookAhead(lookAhead), statePos(statePos) {}

  bool operator<(const StateLine &b) const {
    return vector<int>({genToken, line, statePos }) < vector<int>({ b.genToken, b.line, b.statePos });
  }

  bool operator!=(const StateLine &b) const {
    return vector<int>({genToken, line, statePos }) != vector<int>({ b.genToken, b.line, b.statePos });
  }
};

class TableCreator {
public:
  TableCreator() {
    fillTable();
  }

  vector<vector<int>> getTable() {
    return table;
  }

private:
  unordered_map<int, vector<int>> firsts;
  unordered_map<int, vector<int>> follows;
  map<vector<StateLine>, State> states;

  vector<vector<int>> table;

  vector<int> getLookAhead(int token, vector<int> right, vector<int> lookAhead) {
    vector<int> la;
    bool mayEndWithToken = right[right.size() - 1] == token;

    for (int i = 0; i < right.size() - 1; i++) {
      if (right[i] == token) {
        do {
          for (auto t : getFirst(right[i + 1])) la.push_back(t);
        } while (isNullable(right[i + 1]) && ++i < right.size() - 1);
      }

      mayEndWithToken = mayEndWithToken || i == right.size() - 1;
    }

    if (mayEndWithToken) {
      for (auto t : lookAhead) {
        la.push_back(t);
      }
    }

    sort(la.begin(), la.end());
    la.resize(unique(la.begin(), la.end()) - la.begin());

    // db(token2String(token) _ right _ lookAhead _ la);
    return la;
  }

  vector<int> getFirst(int token) {
    if (firsts.count(token)) {
      return firsts[token];
    }

    if (isBaseToken(token)) {
      return firsts[token] = { token };
    }

    const vector<vector<int>>& rights = rules[token];
    vector<int> first;

    for (const vector<int>& right : rights) {
      if (right.empty()) continue;

      for (auto rightFirst : getFirst(right[0])) {
        first.push_back(rightFirst);
      }
      if (isNullable(right[0])) {
        for (int t : getLookAhead(right[0], right, {})) {
          first.push_back(t);
        }
      }
    }

    sort(first.begin(), first.end());
    first.resize(unique(first.begin(), first.end()) - first.begin());

    return firsts[token] = first;
  }

  bool isNullable(int token) {
    bool nullable = false;
    for (auto v : rules[token])
      nullable = nullable || v.empty();

    return nullable;
  }

  vector<int> getFollow(int token, const pair<int, vector<vector<int>>>& rule, vector<vector<int>>& equals, vector<int>& inStack) {
    vector<int> follow;

    for (const auto& right : rule.second) {
      if (right.empty()) continue;

      bool mayEndWithToken = right[right.size() - 1] == token;

      for (int i = 0; i < right.size() - 1; i++) {
        if (right[i] == token) {
          do {
            for (auto t : getFirst(right[i + 1])) follow.push_back(t);
          } while (isNullable(right[i + 1]) && ++i < right.size() - 1);
        }

        mayEndWithToken = mayEndWithToken || i == right.size() - 1;
      }

      if (mayEndWithToken) {
        try {
          for (auto t : getFollow(rule.first, equals, inStack)) {
            follow.push_back(t);
          }
        }
        catch (string e) {}
      }
    }

    return follow;
  }

  vector<int> getFollow(int token) {
    if (follows.count(token)) return follows[token];

    vector<vector<int>> equals;
    vector<int> inStack;
    getFollow(token, equals, inStack);

    for (auto v : equals) {
      vector<int> follow;
      for (auto x : v) {
        for (auto t : follows[x]) {
          follow.push_back(t);
        }
      }

      sort(follow.begin(), follow.end());
      follow.resize(unique(follow.begin(), follow.end()) - follow.begin());

      for (auto x : v) {
        follows[x] = follow;
      }
    }

    return follows[token];
  }

  vector<int> getFollow(int token, vector<vector<int>>& equals, vector<int>& inStack) {
    if (follows.count(token)) return follows[token];
    vector<int> follow;

    // TODO: Review Logic
    if (count(inStack.begin(), inStack.end(), token)) {
      vector<int> eq = { token };
      for (int i = inStack.size() - 1; inStack[i] != token; i--) {
        eq.push_back(inStack[i]);
      }
      equals.push_back(eq);

      return {};
    }

    inStack.push_back(token);

    for (const auto& rule : rules) {
      for (auto t : getFollow(token, rule, equals, inStack)) {
        follow.push_back(t);
      }
    }

    sort(follow.begin(), follow.end());
    follow.resize(unique(follow.begin(), follow.end()) - follow.begin());

    inStack.pop_back();

    return follows[token] = follow;
  }

  void fillTable() {
    // Must first create all the states to have all lookaheads - Exponential
    fillTableStatesDfs();
    // Now indeed fills the table o(#states)
    table.assign(states.size() + 1, vector<int>(EXTOKEN_END, 0));
    fillTableDfs();
  }

  void fillTableStatesDfs(vector<StateLine> baseStateLines = { StateLine(P2, 0) }) {
    unordered_map<int, vector<StateLine>> next;

    const auto baseStateLine = baseStateLines[0];

    bool isDifferentState = genState(baseStateLines);
    if (!isDifferentState) {
      return;
    }

    auto& state = states[baseStateLines];

    for (const auto &stateLine : state.stateLines) {
      const auto ruleRight = rules[stateLine.genToken][stateLine.line];

      if (ruleRight.size() == stateLine.statePos){
        continue;
      }
      const int nextToken = ruleRight[stateLine.statePos];

      next[nextToken].push_back(StateLine(stateLine.genToken, stateLine.line, stateLine.lookAhead, stateLine.statePos + 1));
    }

    for (auto nextBase : next) {
      fillTableStatesDfs(nextBase.second);
    }

    return;
  }

  int fillTableDfs(vector<StateLine> baseStateLines = { StateLine(P2, 0) }) {
    unordered_map<int, vector<StateLine>> next;

    const auto baseStateLine = baseStateLines[0];

    auto& state = states[baseStateLines];
    if (state.vis) {
      return state.id;
    }
    state.vis = true;

    for (const auto &stateLine : state.stateLines) {
      const auto ruleRight = rules[stateLine.genToken][stateLine.line];

      if (ruleRight.size() == stateLine.statePos) {
        //Reduction
        int reductionId = getReductionId(stateLine.genToken, stateLine.line);
          // db("Reduction" _ state.id _ ruleRight _ stateLine.lookAhead _ token2String(stateLine.genToken) _ stateLine.line);
        for (auto la : stateLine.lookAhead) {
          table[state.id][la] = -reductionId;
        }
        continue;
      }
      const int nextToken = ruleRight[stateLine.statePos];
      next[nextToken].push_back(StateLine(stateLine.genToken, stateLine.line, stateLine.lookAhead, stateLine.statePos + 1));
    }

    for (auto nextBase : next) {
      const auto nextToken = nextBase.first;
      const auto nextBaseLines = nextBase.second;

      int nextStateId = fillTableDfs(nextBase.second);

      table[state.id][nextToken] = nextStateId;
      // db("NextState" _ state.id _ token2String(nextToken) _ nextStateId);
    }

    return state.id;
  }

  bool genState(vector<StateLine> baseStateLines) {
    sort(baseStateLines.begin(), baseStateLines.end());

    vector<StateLine> stateLines;

    for (const auto& baseStateLine : baseStateLines) {
      for (const auto& stateLine : genStateLines(baseStateLine)) {
        stateLines.push_back(stateLine);
      }
    }

    if (states.count(baseStateLines)) {
      for (auto stateLine : states[baseStateLines].stateLines) {
        stateLines.push_back(stateLine);
      }
    }

    auto mergedStateLines = mergeStateLines(stateLines);

    if (haveSameLookAhead(mergedStateLines, states[baseStateLines].stateLines)) {
      return false;
    }

    states[baseStateLines].stateLines = mergedStateLines;

    return true;
  }

  vector<StateLine> genStateLines(const StateLine& stateLine) {
    vector<StateLine> stateLines = { stateLine };
    auto genRuleLine = rules[stateLine.genToken][stateLine.line];

    auto& ruleLines = rules[genRuleLine[stateLine.statePos]];
    for (int i = 0; i < ruleLines.size(); i++) {
      int token = genRuleLine[stateLine.statePos];
      for (auto newStateLine : genStateLines(StateLine(token, i, getLookAhead(token, genRuleLine, stateLine.lookAhead)))) {
        stateLines.push_back(newStateLine);
      }
    }

    return stateLines;
  }

  bool haveSameLookAhead(const vector<StateLine>& a, const vector<StateLine>& b) {
    if (a.size() != b.size()) return false;
    bool allEqual = true;

    for (int i = 0; i < a.size(); i++) {
      allEqual = allEqual && a[i].lookAhead == b[i].lookAhead;
    }

    return allEqual;
  }


  vector<StateLine> mergeStateLines(vector<StateLine>& stateLines) {
    vector<StateLine> mergedLines;
    sort(stateLines.begin(), stateLines.end());

    for (int i = 0; i < stateLines.size(); i++) {
      if (mergedLines.empty() or mergedLines.back() != stateLines[i]) {
        mergedLines.push_back(stateLines[i]);
        continue;
      }

      for (auto t : stateLines[i].lookAhead)
        mergedLines.back().lookAhead.push_back(t);
    }

    for (auto& state : mergedLines) {
      auto& lookAhead = state.lookAhead;
      sort(lookAhead.begin(), lookAhead.end());
      lookAhead.resize(unique(lookAhead.begin(), lookAhead.end()) - lookAhead.begin());
    }

    return mergedLines;
  }

};

