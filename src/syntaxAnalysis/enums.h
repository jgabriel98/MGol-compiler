#ifndef SYNTAX_ANALIZER_ENUMS_HEADER
#define SYNTAX_ANALIZER_ENUMS_HEADER
#include <string>

enum NonTerminalSimbol { P, V, LV, D, TIPO, A, ES, ARG, CMD, LD, OPRD, COND, CABECALHO, EXP_R, CORPO };
enum Action { Reduce, Shift, Accept };


NonTerminalSimbol string_to_NonTerminalSimbol(std::string str);
std::string NonTerminalSimbol_to_string(NonTerminalSimbol simbol);

#endif