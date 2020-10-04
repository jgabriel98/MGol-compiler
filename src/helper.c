#include "helper.h"

void pretty_print_token_attributes(Token_attributes t){
	printf("token:  %s\t",Tokens_to_string(t.token).c_str());
	printf("tipo:   %s\t",Token_types_to_string(t.tipo).c_str());
	printf("lexema: %s\t",t.lexema.c_str());
	//printf("erro:   %s\n\n",t.erro.c_str());
}

void print_token_attributes(Token_attributes t){
	printf("%s\t",Tokens_to_string(t.token).c_str());
	printf("%s\t",Token_types_to_string(t.tipo).c_str());
	printf("%s\t",t.lexema.c_str());
	//printf("%s\n\n",t.erro.c_str());
}

void compact_print_token_attributes(Token_attributes t){
	printf("\n%s\t",Tokens_to_string(t.token).c_str());
	printf("%s\t",t.lexema.c_str());
	/*if(t.erro.size()){
		printf("\t-->%s",t.erro.c_str());
	}*/
}


string Tokens_to_string(Tokens t){
	switch(t){
		case ERRO: return "ERRO";
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
