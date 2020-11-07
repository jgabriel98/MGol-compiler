#ifndef HELPER_HEADER
#define HELPER_HEADER

#include "LexicalAnalizer.h"

void print_token_attributes(Token_attributes t);
void full_print_token_attributes(Token_attributes &t, LexicalAnalizer &scanner, bool split_between_lines = false);

void print_simbols_table(LexicalAnalizer &scanner);

//todo: criar um aquivo enums.h que terá alguns enums, incluindo 'Token', e nele terá também as funções helpers relacionadas abaixo
string Token_to_string(Token t);
Token string_to_Token(string str);

string Token_type_to_string(Token_type t);

#endif