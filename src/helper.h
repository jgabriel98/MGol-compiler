#ifndef HELPER_HEADER
#define HELPER_HEADER

#include "lexicalAnalysis/enums.h"
#include "lexicalAnalysis/LexicalAnalizer.h"
#include "semanticAnalysis/SemanticAnalysis.h"

void print_token_attributes(GrammarSimbol_attributes &t);
void full_print_token_attributes(GrammarSimbol_attributes &t, std::stringstream &scanner_err_buffer, bool split_between_lines = false);

void print_simbols_table(LexicalAnalizer &scanner);

//todo: criar um aquivo enums.h que terá alguns enums, incluindo 'Token', e nele terá também as funções helpers relacionadas abaixo

#endif