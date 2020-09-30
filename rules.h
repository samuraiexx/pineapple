#pragma once

struct Reduction {
  int len;
  int leftToken;
  int line;

  Reduction(int leftToken, int len, int line) : len(len), leftToken(leftToken), line(line) {}
};

// /*
unordered_map<int, vector<vector<int>>> rules = {
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
    {TYPE, ID, EQUALS, ARRAY, LEFT_SQUARE, NUMERAL, RIGHT_SQUARE, OF, T, SEMI_COLON},
    {TYPE, ID, EQUALS, STRUCT, LEFT_BRACES, DC, RIGHT_BRACES, SEMI_COLON},
    {TYPE, ID, EQUALS, T, SEMI_COLON}
  }},
  {DC, {
    {DC, SEMI_COLON, LI, COLON, T },
    {LI, COLON, T}
  }},
  {DF, {{
    FUNCTION, ID, LEFT_PARENTHESIS, LP, RIGHT_PARENTHESIS, COLON, T, B
  }}},
  {LP, {{LP, COMMA, ID, COLON, TYPE}, {ID, COLON, TYPE}}},
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
// */

/** Test firsts and follows
unordered_map<int, vector<vector<int>>> rules =  {
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
   for (int f : getFirst(x)) db("First" _ s[i] _ token2String(f));
   for (int f : getFollow(x)) db("Follow" _ s[i] _ token2String(f));
 }
 // **/

 /** Test table creation
 unordered_map<int, vector<vector<int>>> rules = {
   {P2, {{S, EOF_}}},
   {S, {{L, EQUALS, R}, {R}}},
   {R, {{L}}},
   {L, {{TIMES, R}, {ID}} },
 };
 **/


int getReductionId(int token, int line) {
  return 100 * token + line;
}

Reduction getReductionFromId(int reductionId) {
  const int leftToken = reductionId / 100;
  const int line = reductionId % 100;
  auto rule = rules[leftToken][line];

  return Reduction(leftToken, rule.size(), line);
}
