#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "token.h"

using namespace std;

const int N = 300;

struct TConst {
  char type; // 0-char, 1-int, 2- string
  char cVal;
  int nVal;
  string sVal;
};

struct PSToken {
  PSToken(Token primaryToken, int secondaryToken) : primaryToken(primaryToken), secondaryToken(secondaryToken) {}
  Token primaryToken;
  int secondaryToken;

};

class Tokenizer {
public:
  Tokenizer() {
    string allCharsExcept, allChars;
    for (int c = 0; c < 128; c++) {
      allChars.push_back(c);
      if (c != '"') allCharsExcept.push_back(c);
    }

    trie[1][' '] = 1;
    trie[1]['\n'] = 1;
    trie[1]['\t'] = 1;
    trie[1]['\v'] = 1;
    trie[1]['\f'] = 1;
    trie[1]['\r'] = 1;

    addToken({ "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ" }, Token::ID);
    addToken({ "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-" }, Token::ID, true);
    addToken({ "0123456789" }, Token::NUMERAL, true);
    // addToken({ "0123456789", ".", "0123456789" }, Token::FLOAT_NUMERAL, true);
    addToken({ "\"", allCharsExcept }, Token::UNKNOWN, true);
    addToken({ "\"", allCharsExcept, "\"" }, Token::STRINGVAL);
    addToken({ "'", allChars, "'" }, Token::CHARACTER);

    addStringToken(",", Token::COMMA);
    addStringToken(";", Token::SEMI_COLON);
    addStringToken(":", Token::COLON);
    addStringToken("=", Token::EQUALS);
    addStringToken("[", Token::LEFT_SQUARE);
    addStringToken("]", Token::RIGHT_SQUARE);
    addStringToken("{", Token::LEFT_BRACES);
    addStringToken("}", Token::RIGHT_BRACES);
    addStringToken("(", Token::LEFT_PARENTHESIS);
    addStringToken(")", Token::RIGHT_PARENTHESIS);
    addStringToken("&&", Token::AND);
    addStringToken("||", Token::OR);
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
    addStringToken("\x03", Token::EOF_);

    name2Token["array"] = Token::ARRAY;
    name2Token["bool"] = Token::BOOLEAN;
    name2Token["break"] = Token::BREAK;
    name2Token["char"] = Token::CHAR;
    name2Token["continue"] = Token::CONTINUE;
    name2Token["do"] = Token::DO;
    name2Token["else"] = Token::ELSE;
    name2Token["false"] = Token::FALSE;
    name2Token["function"] = Token::FUNCTION;
    name2Token["if"] = Token::IF;
    name2Token["integer"] = Token::INTEGER;
    // name2Token["float"] = Token::FLOAT;
    name2Token["of"] = Token::OF;
    name2Token["string"] = Token::STRING;
    name2Token["struct"] = Token::STRUCT;
    name2Token["true"] = Token::TRUE;
    name2Token["type"] = Token::TYPE;
    name2Token["var"] = Token::VAR;
    name2Token["while"] = Token::WHILE;
  }

  vector<PSToken> tokenizeCode(string s) {
    vector<PSToken> t;

    int i = 0;
    while (i < s.size()) {
      string word;
      int node = 1;

      while (i < s.size()) {
        char c = s[i++];
        if (!trie[node][c]) {
          i--;
          break;
        }

        node = trie[node][c];
        if (node > 1) {
          word.push_back(c);
        }
      }

      Token primaryToken = name2Token.count(word) ? name2Token[word] : endToken[node];
      int secondaryToken = getSecondaryToken(primaryToken, word);
      t.emplace_back(primaryToken, secondaryToken);
    }

    return t;
  }

  const unordered_map<string, int>& getName2TokenTable() const {
    return name2sToken;
  }

private:
  int trie[N][128] = {};
  int size = 1;
  Token endToken[N];
  vector<TConst> consts;
  unordered_map<string, Token> name2Token;
  unordered_map<string, int> name2sToken;

  int getSecondaryToken(const Token& t, const string &word) {
    switch (t) {
    case(ID):
      return searchName(word);
    case(NUMERAL):
      return addConst(stoi(word));
    case(STRINGVAL):
      return addConst(removeQuotationMarks(word));
    case(CHARACTER):
      return addConst(word[1]);
    default:
      return -1;
    }
  }

  string removeQuotationMarks(const string& s) {
    string ans;
    for (int i = 1; i < s.size() - 1; i++)
      ans.push_back(s[i]);

    return ans;
  }

  int searchName(string var) {
    if(name2sToken.count(var))
      return name2sToken[var];
    return name2sToken[var] = name2sToken.size();
  }

  int addConst(char c) {
    TConst tc;
    tc.type = 0;
    tc.cVal = c;
    consts.push_back(tc);

    return consts.size() - 1;
  }

  int addConst(int x) {
    TConst tc;
    tc.type = 1;
    tc.nVal = x;
    consts.push_back(tc);

    return consts.size() - 1;
  }

  int addConst(string s) {
    TConst tc;
    tc.type = 2;
    tc.sVal = s;
    consts.push_back(tc);

    return consts.size() - 1;
  }

  int getConstInt(int secondaryToken) {
    return consts[secondaryToken].nVal;
  }

  string getConstString(int secondaryToken) {
    return consts[secondaryToken].sVal;
  }

  char getConstChar(int secondaryToken) {
    return consts[secondaryToken].cVal;
  }

  void addStringToken(string pattern, Token token, bool recursive = false) {
    vector<string> p;
    for (auto c : pattern) p.emplace_back(1, c);
    addToken(p, token, recursive);
  }

  void addToken(vector<string> pattern, Token token, bool recursive = false, int node = 1) {
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

    if (nxt == 0 || nxt == node) {
      nxt = ++size;

      for (auto c : s) {
        trie[node][c] = nxt;
      }
    }

    if (pattern.size() == 0 && recursive) {
      for (auto c : s) {
        trie[nxt][c] = nxt;
      }
    }

    addToken(pattern, token, recursive, nxt);
  }

};
