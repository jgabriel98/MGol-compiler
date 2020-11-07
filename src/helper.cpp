#include "helper.h"
#include <iostream>
#include "utils.h"
#include <regex>

void print_token_attributes(Token_attributes t){
	printf("%s\t",Token_to_string(t.token).c_str());
	printf("%s\t",Token_type_to_string(t.tipo).c_str());
	printf("%s\t",t.lexema.c_str());
}

void full_print_token_attributes(Token_attributes &t, LexicalAnalizer &scanner, bool split_between_lines){
	string lexema = string_utils::wrap(t.lexema, 20, 20);
	const char colum_style = split_between_lines ? ' ' : '|';
	printf("%-20s %c %10.10s %c %10.10s %c ", lexema.c_str(), colum_style, 
											  Token_to_string(t.token).c_str(), colum_style, 
											  Token_type_to_string(t.tipo).c_str(), colum_style);
	
	if(scanner.error_s.peek() != EOF){
		char buff;
		while(!scanner.error_s.get(buff).eof()){
			printf("%c", buff);
			if(buff == '\n')
				printf("%49.49s", "");
		}
		
	}

	if(split_between_lines)
		printf("\n-------------------- | ---------- | ---------- | ------------------------------------- - - -  -  -  -");

	printf("\n");
}

void print_simbols_table(LexicalAnalizer &scanner) {
	printf("********** TABELA DE SÍMBOLOS ***********\n");
	printf("%-15.15s | %10.10s | %10.10s\n", "lexema", "token", "tipo");
	printf("--------------- | ---------- | ----------\n");
	for(auto &simbol: scanner.simbols_table){
		//Token_attributes &simbol = l;
		printf("%-15.15s | %10.10s | %10.10s\n", simbol.lexema.c_str(), Token_to_string(simbol.token).c_str(), Token_type_to_string(simbol.tipo).c_str());
	}
}

string Token_to_string(Token t){
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
		case Num: return "Num";
		case Literal: return "Literal";
		case id: return "id";
		case Comentario: return "Comentario";
		case Espaco: return "Espaco";
		case EOF_t: return "EOF";
		case OPR: return "OPR";
		case RCB: return "RCB";
		case OPM: return "OPM";
		case AB_P: return "AB_P";
		case FC_P: return "FC_P";
		case PT_V: return "PT_V";
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
	if(str == "Num" ) return Num;
	if(str == "Literal" ) return Literal;
	if(str == "id" ) return id;
	if(str == "EOF" ) return EOF_t;
	if(str == "OPR" ) return OPR;
	if(str == "RCB" ) return RCB;
	if(str == "OPM" ) return OPM;
	if(str == "(" || str == "AB_P" ) return AB_P;
	if(str == ")" || str == "FC_P" ) return FC_P;
	if(str == ";" || str == "PT_V" ) return PT_V;
	
	throw runtime_error("Não foi possível converter a string '"+str+"' para o tipo"+typeid(Token).name());
}

string Token_type_to_string(Token_type t) {
    switch(t){
        case unknow: return "-";
        case Inteiro: return "Inteiro";
        case Real: return "Real";
        case SCI_NUM: return "SCI_NUM";
    }
}
