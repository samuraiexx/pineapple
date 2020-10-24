#include "rules.h"
#include "token.h"

using namespace std;

int getReductionId(int token, int line) {
  return 100 * token + line;
}

int getReductionId(Reduction reduction) {
  return 100 * reduction.leftToken + reduction.line;
}

Reduction getReductionFromId(int reductionId) {
  const int leftToken = reductionId / 100;
  const int line = reductionId % 100;
  auto rule = rules[leftToken][line];

  return Reduction(leftToken, rule.size(), line);
}

// /*
unordered_map<int, vector<vector<int>>> rules = {
   {P2, {{P, EOF_}}},
   {P, {{LDE }}},
   {LDE, {{LDE, DE}, {DE}}},
   {DE, {{DF}, {DT} }},
   {T, {
     {INTEGER},
     {CHAR},
     {BOOLEAN},
     {STRING},
     {IDU}
   }},
   {DT, {
     {TYPE, IDD, EQUALS, ARRAY, LEFT_SQUARE, NUMERAL, RIGHT_SQUARE, OF, T, SEMI_COLON},
     {TYPE, IDD, EQUALS, STRUCT, NB, LEFT_BRACES, DC, RIGHT_BRACES, SEMI_COLON},
     {TYPE, IDD, EQUALS, T, SEMI_COLON}
   }},
   {DC, {
     {DC, SEMI_COLON, LI, COLON, T },
     {LI, COLON, T}
   }},
   {DF, {{
     FUNCTION, IDD, NB, LEFT_PARENTHESIS, LP, RIGHT_PARENTHESIS, COLON, T, MF, B
   }}},
   {LP, {{LP, COMMA, IDD, COLON, T}, {IDD, COLON, T}}},
   {B, {{LEFT_BRACES, LDV, LS, RIGHT_BRACES}}},
   {LDV, {{LDV, DV}, {DV}}},
   {LS, {{LS, S}, {S}}},
   {DV, {{VAR, LI, COLON, T, SEMI_COLON}}},
   {LI, {{LI, COMMA, IDD}, {IDD}}},
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
     {LV},
     {PLUS_PLUS, LV },
     {MINUS_MINUS, LV},
     {LV, PLUS_PLUS},
     {LV, MINUS_MINUS},
     {LEFT_PARENTHESIS, E, RIGHT_PARENTHESIS},
     {IDU, LEFT_PARENTHESIS, LE, RIGHT_PARENTHESIS},
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
   {LV, {{LV, DOT, _ID}, {LV, LEFT_SQUARE, E, RIGHT_SQUARE}, {IDU}}},
   {NB, {{}}},
   {IDD, {{ID}}},
   {IDU, {{ID}}},
   {_ID, {{ID}}},
   {_TRUE, {{TRUE}}},
   {_FALSE, {{FALSE}}},
   {_CHARACTER, {{CHARACTER}}},
   {_STRINGVAL, {{STRINGVAL}}},
   {_NUMERAL, {{NUMERAL}}},
   {MF, {{}}},
   {MC, {{}}},
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
