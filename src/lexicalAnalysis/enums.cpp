#include "enums.h"
#include <stdexcept>


string Token_to_string(Token t) {
	switch(t){
		case ERRO: return "ERRO";
		case inicio: return "inicio";
		case varinicio: return "varinicio";
		case varfim: return "varfim";
		case escreva: return "escreva";
		case leia: return "leia";
		case se: return "se";
		case entao: return "entao";
		case fimse: return "fimse";
		case fim: return "fim";
		case inteiro: return "inteiro";
		case lit: return "lit";
		case real: return "real";
		case num: return "num";
		case literal: return "literal";
		case id: return "id";
		case Comentario: return "Comentario";
		case Espaco: return "Espaco";
		case EOF_t: return "EOF";
		case opr: return "opr";
		case Token::rcb: return "rcb";
		case opm: return "opm";
		case AB_P: return /*"AB_P"*/"(";
		case FC_P: return /*"FC_P"*/")";
		case PT_V: return /*"PT_V"*/";";
	}
	return nullptr;
}


string Token_type_to_string(Token_type t) {
    switch(t){
        case unknow: return "?";
        case Inteiro: return "Inteiro";
        case Real: return "Real";
        case Cientifico: return "Cientifico";
		case Literal: return "Literal";
		case opm_soma: return "+";
		case opm_sub: return "-";
		case opm_mult: return "*";
		case opm_div: return "/";
		case opr_eq: return "=";
		case opr_neq: return "<>";
		case opr_less: return "<";
		case opr_greater: return ">";
		case opr_less_eq: return "<=";
		case opr_greater_eq: return ">=";
		case atr: return "<-";
    }
	return nullptr;
}


Token string_to_Token(string str) {
	
	if(str == "inicio" ) return inicio;
	if(str == "varinicio" ) return varinicio;
	if(str == "varfim" ) return varfim;
	if(str == "escreva" ) return escreva;
	if(str == "leia" ) return leia;
	if(str == "se" ) return se;
	if(str == "entao" ) return entao;
	if(str == "fimse" ) return fimse;
	if(str == "fim" ) return fim;
	if(str == "inteiro" ) return inteiro;
	if(str == "lit" ) return lit;
	if(str == "real" ) return real;
	if(str == "num" ) return num;
	if(str == "literal" ) return literal;
	if(str == "id" ) return id;
	if(str == "EOF" || str == "$" ) return EOF_t;
	if(str == "opr" ) return opr;
	if(str == "rcb" ) return rcb;
	if(str == "opm" ) return opm;
	if(str == "(" || str == "AB_P" ) return AB_P;
	if(str == ")" || str == "FC_P" ) return FC_P;
	if(str == ";" || str == "PT_V" ) return PT_V;
	if(str == "ERRO") return ERRO;
	
	throw runtime_error("Não foi possível converter a string '"+str+"' para o tipo "+typeid(Token).name());
}