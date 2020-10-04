#ifndef HELPER_HEADER
#define HELPER_HEADER

#include "LexicalAnalizer.h"

void pretty_print_token_attributes(Token_attributes t);
void print_token_attributes(Token_attributes t);
void compact_print_token_attributes(Token_attributes t);


string Tokens_to_string(Tokens t);

string Token_types_to_string(Token_types t);

#endif