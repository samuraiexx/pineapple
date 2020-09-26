#pragma once
#include <vector>
#include <unordered_map>
#include <cstdarg>

#include "token.h"

using namespace std;

class Parser {
public:
  Parser() {
    rules = {
      {P2, {{P, EOF_}}},
      {P, {{LDE }}},
      {LDE, {{LDE, DE}, {DE}}},
      {DE, {{DF}, {DT} }},
      {T, {{INTEGER}}},
      {T, {{CHAR}}},
      {T, {{BOOLEAN}}},
      {T, {{STRING}}},
      {T, {{ID}}},
      {DT, {
        {T, ID, EQUALS, ARRAY, LEFT_SQUARE, NUMERAL, RIGHT_SQUARE, OF, T},
        {TYPE, ID, EQUALS, STRUCT, LEFT_BRACES, DC, RIGHT_BRACES},
        {TYPE, ID, EQUALS, T}
      }},
      {DC, {
        {DC, SEMI_COLON, LI, COLON, T },
        {LI, COLON, T}
      }},
      {DF, {{
        FUNCTION, ID, LEFT_PARENTHESIS, LP, RIGHT_PARENTHESIS, COLON, T, B
      }}},
      {LP, {{LP, ID, COLON, TYPE}, {ID, COLON, TYPE}}},
      {B, {{LEFT_BRACES, LDV, LS, RIGHT_BRACES}}},
      {LDV, {{LDV, DV}, {DV}}},
      {LS, {{LS, S}, {S}}},
      {DV, {{VAR, LI, COLON, T, SEMI_COLON}}},
      {LI, {{LI, COMMA, ID}, {ID}}},
      {S, {
        {IF, LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS, S},
        {IF, LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS, S, ELSE, S},
        {WHILE, LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS, S},
        {DO, S, WHILE, LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS, SEMI_COLON},
        {B},
        {LV, EQUALS, E, SEMI_COLON},
        {BREAK, SEMI_COLON},
        {CONTINUE, SEMI_COLON}
      }},
      {E, {{E, AND, L}, {E, OR, L}, {L}}},
      {L, {
      {L, LESS_THAN, R},
      {L, GREATER_THAN, R},
      {L, LESS_OR_EQUAL, R},
      {L, GREATER_OR_EQUAL, R},
      {L, EQUAL_EQUAL, R},
      {L, NOT_EQUAL, R},
      {R}
      }},
      {R, {{L, PLUS, Y}, {L, MINUS, Y}, {Y}}},
      {Y, {{Y, TIMES, F}, {Y, DIVIDE, F}, {F}}},
      {F, {
        {PLUS_PLUS, LV },
        {MINUS_MINUS, LV},
        {LV, PLUS_PLUS},
        {LV, MINUS_MINUS},
        {LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS},
        {ID, LEFT_PARENTHESIS, LE, RIGHT_PARENTHESIS},
        {MINUS, F},
        {NOT, F},
        {TRUE},
        {FALSE},
        {CHARACTER},
        {STRINGVAL},
        // {FLOAT_NUMERAL},
        {NUMERAL}
      }},
      {LE, {{LE, COMMA, E}, {E} }},
      {LV, {{LV, DOT, ID}, {LV, LEFT_SQUARE, E, RIGHT_SQUARE}, {ID}}},
    };

    /** Test firsts and follows
    rules.clear();
    rules = { 
      {S, {{E, EOF_}}},
      {E, {{T, DE}}},
      {DE, {{PLUS, T, DE}, {}}},
      {T, {{F, DF}} },
      {DF, {{TIMES, F, DF}, {}}},
      {F, {{LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS}, {ID}}}
    };

    int v[] = { S, E, DE, T, DF, F };
    for (int i = 0; i < 6; i++) {
      auto x = v[i];
      string s[] = { "S", "E", "DE", "T", "DF", "F" };
      for (int f : getFirst(x)) db("First" _ s[i] _ token2String((Token)f));
      for (int f : getFollow(x)) db("Follow" _ s[i] _ token2String((Token)f));
    }
    **/
  }

  void parse(vector<Token> tokens) {
  }

private:
  unordered_map<int, vector<vector<int>>> rules;
  unordered_map<int, vector<int>> firsts;
  unordered_map<int, vector<int>> follows;

  vector<int> getLookAhead(int token, pair<int, vector<vector<int>>> rule, vector<int> lookAhead) {
    vector<int> la;
    for (const auto& right : rule.second) {
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
    }

    sort(la.begin(), la.end());
    la.resize(unique(la.begin(), la.end()) - la.begin());
    return la;
  }

  vector<int> getFirst(int token) {
    if (firsts.count(token)) {
      return firsts[token];
    }

    if (!isExtendedToken(token)) {
      return firsts[token] = { token };
    }

    const vector<vector<int>> &rights = rules[token];
    vector<int> first;

    for (const vector<int> &right : rights) {
      if (right.empty()) continue;

      for (auto rightFirst : getFirst(right[0])) {
        first.push_back(rightFirst);
      }
      if (isNullable(right[0])) {
        for (int t : getLookAhead(right[0], { token, rights }, {})) {
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

  inline bool isExtendedToken(int token) {
    return token >= TOKEN_END;
  }

  /**
  **/
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
};
