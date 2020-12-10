#include "helper.h"
#include <sstream>
#include "utils.h"
//#include <regex>

#include "semanticAnalysis/SemanticAnalysis.h"

void print_token_attributes(GrammarSimbol_attributes &t){
	printf("%s\t",Token_to_string(t.token.simbol.asTerminal).c_str());
	printf("%s\t",Token_type_to_string(t.tipo).c_str());
	printf("%s\t",t.lexema.c_str());
}

void full_print_token_attributes(GrammarSimbol_attributes &t, std::stringstream &scanner_err_buffer, bool split_between_lines){
	string lexema = string_utils::wrap(t.lexema, 20, 20);
	const char colum_style = split_between_lines ? ' ' : '|';
	printf("%-20s %c %10.10s %c %10.10s %c ", lexema.c_str(), colum_style, 
											  Token_to_string(t.token.simbol.asTerminal).c_str(), colum_style, 
											  Token_type_to_string(t.tipo).c_str(), colum_style);
	
	
	while(scanner_err_buffer.peek() != EOF){
		char buff;
		scanner_err_buffer.get(buff);
		printf("%c", buff);
		if(buff == '\n')
			printf("%49.49s", "");
		
	}

	if(split_between_lines)
		printf("\n-------------------- | ---------- | ---------- | -------------------------------------");

	printf("\n");
}

void print_simbols_table(LexicalAnalizer &scanner) {
	printf("********** TABELA DE SÍMBOLOS ***********\n");
	printf("%-15.15s | %10.10s | %10.10s\n", "lexema", "token", "tipo");
	printf("--------------- | ---------- | ----------\n");
	for(auto &simbol: scanner.simbols_table){
		//Token_attributes &simbol = l;
		printf("%-15.15s | %10.10s | %10.10s\n", simbol->lexema.c_str(), Token_to_string(simbol->token.simbol.asTerminal).c_str(), Token_type_to_string(simbol->tipo).c_str());
	}
}


