#include "enums.h"
#include <stdexcept>

NonTerminalSimbol string_to_NonTerminalSimbol(std::string str) {
	if (str == "P'")			return P_;
	if (str == "P")				return P;
	if (str == "V")				return V;
	if (str == "LV")			return LV;
	if (str == "D")				return D;
	if (str == "TIPO")			return TIPO;
	if (str == "A")				return A;
	if (str == "ES")			return ES;
	if (str == "ARG")			return ARG;
	if (str == "CMD")			return CMD;
	if (str == "LD")			return LD;
	if (str == "OPRD")			return OPRD;
	if (str == "COND")			return COND;
	if (str == "CABECALHO")		return CABECALHO;
	if (str == "EXP_R")			return EXP_R;
	if (str == "CORPO")			return CORPO;

	throw std::runtime_error("Não foi possível converter a string '" + str + "' para o tipo " +
							 typeid(NonTerminalSimbol).name());
}

std::string NonTerminalSimbol_to_string(NonTerminalSimbol simbol) {
	if (simbol == P_)			return "P'";
	if (simbol == P)			return "P";
	if (simbol == V)			return "V";
	if (simbol == LV)			return "LV";
	if (simbol == D)			return "D";
	if (simbol == TIPO)			return "TIPO";
	if (simbol == A)			return "A";
	if (simbol == ES)			return "ES";
	if (simbol == ARG)			return "ARG";
	if (simbol == CMD)			return "CMD";
	if (simbol == LD)			return "LD";
	if (simbol == OPRD)			return "OPRD";
	if (simbol == COND)			return "COND";
	if (simbol == CABECALHO)	return "CABECALHO";
	if (simbol == EXP_R)		return "EXP_R";
	if (simbol == CORPO)		return "CORPO";

	return nullptr;
}