#ifndef HELPER_HEADER
#define HELPER_HEADER

#include "LexicalAnalizer.h"

void print_token_attributes(Token_attributes t);
void full_print_token_attributes(Token_attributes &t, LexicalAnalizer &scanner);

void print_simbols_table(LexicalAnalizer &scanner);


string Tokens_to_string(Tokens t);

string Token_types_to_string(Token_types t);

#endif