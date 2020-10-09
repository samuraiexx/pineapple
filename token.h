#pragma once
#include<string>

bool isBaseToken(int t);
std::string token2String(int t);

enum Token {
  UNKNOWN,
  ARRAY, BOOLEAN, BREAK, CHAR, CONTINUE, DO, ELSE, FALSE, FUNCTION, IF,
  INTEGER, FLOAT, OF, STRING, STRUCT, TRUE, TYPE, VAR, WHILE,
  COLON, SEMI_COLON, COMMA, EQUALS, LEFT_SQUARE, RIGHT_SQUARE,
  LEFT_BRACES, RIGHT_BRACES, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, AND,
  OR, LESS_THAN, GREATER_THAN, LESS_OR_EQUAL, GREATER_OR_EQUAL,
  NOT_EQUAL, EQUAL_EQUAL, PLUS, PLUS_PLUS, MINUS, MINUS_MINUS, TIMES,
  DIVIDE, DOT, NOT,
  CHARACTER, NUMERAL, STRINGVAL, ID, // FLOAT_NUMERAL, 
  EOF_,
  TOKEN_END,
};

enum ExToken {
  P = Token::TOKEN_END, LDE, DE, DF, DT, T, B, LDV, LS,
  DV, LI, E, S, LV, L, R, Y, F, LP, LE, DC, P2, NB, IDD, IDU,
  _ID, _FALSE, _TRUE, _CHARACTER, _STRINGVAL, _NUMERAL,
  EXTOKEN_END
};

