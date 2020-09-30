#pragma once
#include<string>

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
  DV, LI, E, S, LV, L, R, Y, F, LP, LE, DC, P2,
  EXTOKEN_END
};

bool isBaseToken(int t) {
  return t < TOKEN_END;
}

std::string token2String(int t) {
  switch (t) {
  case Token::ARRAY:
    return "ARRAY";
  case Token::BOOLEAN:
    return "BOOLEAN";
  case Token::BREAK:
    return "BREAK";
  case Token::CHAR:
    return "CHAR";
  case Token::CONTINUE:
    return "CONTINUE";
  case Token::DO:
    return "DO";
  case Token::ELSE:
    return "ELSE";
  case Token::FALSE:
    return "FALSE";
  case Token::FUNCTION:
    return "FUNCTION";
  case Token::IF:
    return "IF";
  case Token::INTEGER:
    return "INTEGER";
  case Token::FLOAT:
    return "FLOAT";
  case Token::OF:
    return "OF";
  case Token::STRING:
    return "STRING";
  case Token::STRUCT:
    return "STRUCT";
  case Token::TRUE:
    return "TRUE";
  case Token::TYPE:
    return "TYPE";
  case Token::VAR:
    return "VAR";
  case Token::WHILE:
    return "WHILE";
  case Token::COLON:
    return "COLON";
  case Token::SEMI_COLON:
    return "SEMI_COLON";
  case Token::COMMA:
    return "COMMA";
  case Token::EQUALS:
    return "EQUALS";
  case Token::LEFT_SQUARE:
    return "LEFT_SQUARE";
  case Token::RIGHT_SQUARE:
    return "RIGHT_SQUARE";
  case Token::LEFT_BRACES:
    return "LEFT_BRACES";
  case Token::RIGHT_BRACES:
    return "RIGHT_BRACES";
  case Token::LEFT_PARENTHESIS:
    return "LEFT_PARENTHESIS";
  case Token::RIGHT_PARENTHESIS:
    return "RIGHT_PARENTHESIS";
  case Token::AND:
    return "AND";
  case Token::OR:
    return "OR";
  case Token::LESS_THAN:
    return "LESS_THAN";
  case Token::GREATER_THAN:
    return "GREATER_THAN";
  case Token::LESS_OR_EQUAL:
    return "LESS_OR_EQUAL";
  case Token::GREATER_OR_EQUAL:
    return "GREATER_OR_EQUAL";
  case Token::NOT_EQUAL:
    return "NOT_EQUAL";
  case Token::EQUAL_EQUAL:
    return "EQUAL_EQUAL";
  case Token::PLUS:
    return "PLUS";
  case Token::PLUS_PLUS:
    return "PLUS_PLUS";
  case Token::MINUS:
    return "MINUS";
  case Token::MINUS_MINUS:
    return "MINUS_MINUS";
  case Token::TIMES:
    return "TIMES";
  case Token::DIVIDE:
    return "DIVIDE";
  case Token::DOT:
    return "DOT";
  case Token::NOT:
    return "NOT";
  case Token::CHARACTER:
    return "CHARACTER";
  case Token::NUMERAL:
    return "NUMERAL";
  case Token::STRINGVAL:
    return "STRINGVAL";
  case Token::ID:
    return "ID";
  // case FLOAT_NUMERAL:
  // return "NUMERAL";
  case Token::EOF_:
    return "EOF_";
  case ExToken::P:
    return "P";
  case ExToken::LDE:
    return "LDE";
  case ExToken::DE:
    return "DE";
  case ExToken::DF:
    return "DF";
  case ExToken::DT:
    return "DT";
  case ExToken::T:
    return "T";
  case ExToken::B:
    return "B";
  case ExToken::LDV:
    return "LDV";
  case ExToken::LS:
    return "LS";
  case ExToken::DV:
    return "DV";
  case ExToken::LI:
    return "LI";
  case ExToken::E:
    return "E";
  case ExToken::S:
    return "S";
  case ExToken::LV:
    return "LV";
  case ExToken::L:
    return "L";
  case ExToken::R:
    return "R";
  case ExToken::Y:
    return "Y";
  case ExToken::F:
    return "F";
  case ExToken::LP:
    return "LP";
  case ExToken::LE:
    return "LE";
  case ExToken::DC:
    return "DC";
  case ExToken::P2:
    return "P2";
  case ExToken::EXTOKEN_END:
    return "EXTOKEN_END";
  case Token::UNKNOWN:
  default:
    return "UNKNOWN";
  }
}
