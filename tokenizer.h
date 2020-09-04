#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

const int N = 1e3;

class Tokenizer {
  int trie[N][128];
  int endToken[N];
  int size = 1;

  enum Token {
    // palavras reservadas
    ARRAY, BOOLEAN, BREAK, CHAR, CONTINUE, DO, ELSE, FALSE, FUNCTION, IF,
    INTEGER, FLOAT, OF, STRING, STRUCT, TRUE, TYPE, VAR, WHILE,
    COLON, SEMI_COLON, COMMA, EQUALS, LEFT_SQUARE, RIGHT_SQUARE,
    LEFT_BRACES, RIGHT_BRACES, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, AND,
    OR, LESS_THAN, GREATER_THAN, LESS_OR_EQUAL, GREATER_OR_EQUAL,
    NOT_EQUAL, EQUAL_EQUAL, PLUS, PLUS_PLUS, MINUS, MINUS_MINUS, TIMES,
    DIVIDE, DOT, NOT,
    CHARACTER, INT_NUMERAL, FLOAT_NUMERAL, STRINGVAL, ID,
    UNKNOWN
  };

  Tokenizer() {
    string allCharsExcept, allChars;
    for (int c = 0; c < 128; c++) {
      allChars.push_back(c);
      if (c != '"') allCharsExcept.push_back(c);
    }

    addToken({"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-"}, Token::ID, true);
    addToken({ "0123456789" }, Token::INT_NUMERAL, true);
    addToken({ "0123456789", ".", "0123456789" }, Token::FLOAT_NUMERAL, true);
    addToken({ "\"", allCharsExcept }, Token::UNKNOWN, true);
    addToken({ "\"", allCharsExcept, "\""}, Token::STRINGVAL);
    addToken({ "'", allChars, "'" }, Token::CHARACTER);

    addStringToken("Array", Token::ARRAY);
    addStringToken("bool", Token::BOOLEAN);
    addStringToken("break", Token::BREAK);
    addStringToken("char", Token::CHAR);
    addStringToken("continue", Token::CONTINUE);
    addStringToken("do", Token::DO);
    addStringToken("else", Token::ELSE);
    addStringToken("false", Token::FALSE);
    addStringToken("function", Token::FUNCTION);
    addStringToken("if", Token::IF);
    addStringToken("int", Token::INTEGER);
    addStringToken("float", Token::FLOAT);
    addStringToken("of", Token::OF);
    addStringToken("string", Token::STRING);
    addStringToken("struct", Token::STRUCT);
    addStringToken("true", Token::TRUE);
    addStringToken("type", Token::TYPE);
    addStringToken("var", Token::VAR);
    addStringToken("while", Token::WHILE);
    addStringToken(",", Token::COLON);
    addStringToken(";", Token::SEMI_COLON);
    addStringToken(":", Token::COMMA);
    addStringToken("=", Token::EQUALS);
    addStringToken("[", Token::LEFT_SQUARE);
    addStringToken("]", Token::RIGHT_SQUARE);
    addStringToken("{", Token::LEFT_BRACES);
    addStringToken("}", Token::RIGHT_BRACES);
    addStringToken("(", Token::LEFT_PARENTHESIS);
    addStringToken(")", Token::RIGHT_PARENTHESIS);
    addStringToken("and", Token::AND);
    addStringToken("or", Token::OR);
    addStringToken("<", Token::LESS_THAN);
    addStringToken(">", Token::GREATER_THAN);
    addStringToken("<=", Token::LESS_OR_EQUAL);
    addStringToken(">=", Token::GREATER_OR_EQUAL);
    addStringToken("!=", Token::NOT_EQUAL);
    addStringToken("==", Token::EQUALS);
    addStringToken("+", Token::PLUS);
    addStringToken("++", Token::PLUS_PLUS);
    addStringToken("-", Token::MINUS);
    addStringToken("--", Token::MINUS_MINUS);
    addStringToken("*", Token::TIMES);
    addStringToken("/", Token::DIVIDE);
    addStringToken(".", Token::DOT);
    addStringToken("!", Token::NOT);
  }

  void addStringToken(string pattern, Token token, bool recursive = false) {
    vector<string> p;
    for (auto c : pattern) p.emplace_back(1, c);
    addToken(p, token, false);
  }

  void addToken(vector<string> pattern, Token token, bool recursive = false, int node = 1, bool copyLast = false) {
    if (node == 1) {
      reverse(pattern.begin(), pattern.end());
    }

    if (pattern.size() == 0) {
      endToken[node] = token;
      return;
    }

    string s = pattern.back();
    pattern.pop_back();

    int nxt = trie[node][s[0]];

    if (nxt == node) {
      copyLast = true;
    }

    if (nxt == 0 || nxt == node) {
      nxt = ++size;

      for (auto c : s) {
        trie[node][c] = nxt;
      }

      if (copyLast) {
        for (int i = 0; i < 128; i++) {
          trie[nxt][i] = trie[node][i];
          if (trie[nxt][i] == node) {
            trie[nxt][i] = nxt;
          }
        }

        endToken[nxt] = endToken[node];
      }
    }

    if (pattern.size() == 0 && recursive) {
      for (auto c : s) {
        trie[nxt][c] = nxt;
      }
    }

    addToken(pattern, token, recursive, nxt, copyLast);
  }

  vector<int> tokenizeString(string t) {
  }
};
