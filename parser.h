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
      { P, {{LDE } } },
      {LDE, {{LDE, DE}, {DE}}},
      {DE, {{DF}, {DT} }},
      {T, {{INTEGER}}},
      {T, {{CHAR}}},
      {T, {{BOOLEAN}}},
      {T, {{STRING}}},
      {T, {{ID}}},
      /*
      {DT, {
        {T, ID, EQUALS, ARRAY, LEFT_SQUARE, INT_NUMERAL, RIGHT_SQUARE}
      }},
      {DT, {{}}};
      {DC, {}},
      */
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
        {FLOAT_NUMERAL},
        {INT_NUMERAL}
      }},
      {LE, {{LE, COMMA, E}, {E} }},
      {LV, {{LV, DOT, ID}, {LV, LEFT_SQUARE, E, RIGHT_SQUARE}, {ID}}},
    };
  }
  void parse(vector<Token> tokens) {
  }
private:
  unordered_map<int, vector<vector<int>>> rules;
  unordered_map<int, vector<int>> firsts;
  unordered_map<int, vector<int>> follows;

  vector<int> getFlollow(int token) {
  }

  vector<int> getFirst(int token) {
    if (firsts.count(token)) {
      return firsts[token];
    }

    if (!isExtendedToken(token)) {
      return firsts[token] = { token };
    }

    auto rights = rules[token];
    vector<int> first;

    for (vector<int> right : rights) {
      for (auto rightFirst: getFirst(right[0])) {
        first.push_back(rightFirst);
      }
    }

    return firsts[token] = first;
  }

  inline bool isExtendedToken(int token) {
    return token >= TOKEN_END;
  }
};
