#include "helper.h"
#include <iostream>

void print_token_attributes(Token_attributes t){
	printf("%s\t",Tokens_to_string(t.token).c_str());
	printf("%s\t",Token_types_to_string(t.tipo).c_str());
	printf("%s\t",t.lexema.c_str());
}

void full_print_token_attributes(Token_attributes &t, LexicalAnalizer &scanner){
	printf("%-15.15s | %10.10s | %10.10s | ", t.lexema.c_str(), Tokens_to_string(t.token).c_str(), Token_types_to_string(t.tipo).c_str());
	if(scanner.error_s.peek() != EOF){
		cout << scanner.error_s.rdbuf();
	}
	printf("\n");
}

void print_simbols_table(LexicalAnalizer &scanner) {
	printf("%-15.15s | %10.10s | %10.10s\n", "lexema", "token", "tipo");
	printf("--------------- | ---------- | ----------\n");
	for(auto &l: scanner.simbols_table){
		Token_attributes &simbol = l.second;
		printf("%-15.15s | %10.10s | %10.10s\n", simbol.lexema.c_str(), Tokens_to_string(simbol.token).c_str(), Token_types_to_string(simbol.tipo).c_str());
	}
}

string Tokens_to_string(Tokens t){
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
		case EOF_t: return "EOF";
		case OPR: return "OPR";
		case RCB: return "RCB";
		case OPM: return "OPM";
		case AB_P: return "AB_P";
		case FC_P: return "FC_P";
		case PT_V: return "PT_V";
	}
}

string Token_types_to_string(Token_types t) {
    switch(t){
        case unknow: return "unknow";
        case Inteiro: return "Inteiro";
        case Real: return "Real";
        case SCI_NUM: return "SCI_NUM";
    }
}
