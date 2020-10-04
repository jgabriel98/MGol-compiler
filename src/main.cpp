#include "LexicalAnalizer.h"
#include "helper.h"

#include <stdio.h>
//#include <sstream>		//provavelmente nao esta usando, necessário apenas para usar a classe istringstream
#include <fstream>
#include <iostream>


enum Estados{ S0=0, SPV, SPAR_0, SPAR_1, SOPR_1, SOPR_2, SOPR_3, SOPR_4, SATR, SOPA, SL_0, SL_1, SEOF, SC_0, SC_1, SID, SN_0, SN_1, SN_2, SN_1_2, SN_3, SN_4, SB,
			  REJECT = -1
};

const vector<char> DIGITOS = {'0','1','2','3','4','5','6','7','8','9'};
const vector<char> LETRAS = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};


void configura_analisador_lexico(LexicalAnalizer &lexical_analizer);
void preenche_tabela_de_simbolos(LexicalAnalizer &lexical_analizer);




int main() {

    LexicalAnalizer scanner = LexicalAnalizer(S0, REJECT);
	configura_analisador_lexico(scanner);
	preenche_tabela_de_simbolos(scanner);

	Token_attributes token;
	//istringstream istr("es");
	std::ifstream istr("example.mgol");

	printf("%-20s | %10s | %10s | %s\n", "lexema", "token", "tipo", "erro");
	printf("-------------------- | ---------- | ---------- | \n");
	do {
		token = scanner.analyze(istr);
		//imprimindo token
		full_print_token_attributes(token, scanner);
	} while (token.token != Tokens::EOF_t);
	

	printf("\n\n");
	print_simbols_table(scanner);

	return 0;
}


void preenche_tabela_de_simbolos(LexicalAnalizer &lexical_analizer) {
	lexical_analizer.simbols_table["inicio"]	=	{Tokens::inicio,	Token_types::unknow, "inicio"};
	lexical_analizer.simbols_table["varinicio"]	=	{Tokens::varinicio,	Token_types::unknow, "varinicio"};
	lexical_analizer.simbols_table["varfim"]	=	{Tokens::varfim,	Token_types::unknow, "varfim"};
	lexical_analizer.simbols_table["escreva"]	=	{Tokens::escreva,	Token_types::unknow, "escreva"};
	lexical_analizer.simbols_table["leia"]		=	{Tokens::leia,		Token_types::unknow, "leia"};
	lexical_analizer.simbols_table["se"]		=	{Tokens::se,		Token_types::unknow, "se"};
	lexical_analizer.simbols_table["entao"]		=	{Tokens::entao,		Token_types::unknow, "entao"};
	lexical_analizer.simbols_table["fimse"]		=	{Tokens::fimse,		Token_types::unknow, "fimse"};
	lexical_analizer.simbols_table["fim"]		=	{Tokens::fim,		Token_types::unknow, "fim"};
	lexical_analizer.simbols_table["inteiro"]	=	{Tokens::inteiro,	Token_types::unknow, "inteiro"};
	lexical_analizer.simbols_table["lit"]		=	{Tokens::lit,		Token_types::unknow, "lit"};
	lexical_analizer.simbols_table["real"]		=	{Tokens::real,		Token_types::unknow, "real"};
}

void configura_analisador_lexico(LexicalAnalizer &scanner) {

	/********** Criando trasições de estados **********/
	//parenteses
    scanner.add_transition(S0,          SPAR_0,         '(');
    scanner.add_transition(S0,          SPAR_1,         ')');

    scanner.add_transition(S0,          SPV,            ';');
	//operadores aritmeticos
    scanner.add_transition(S0,          SOPA,           {'+','-','*','/'});

	//operadores relacionais, e atribuição (SATR)
    scanner.add_transition(S0,          SOPR_1,         '<');
    scanner.add_transition(SOPR_1,      SOPR_2,         {'=', '>'});
    scanner.add_transition(SOPR_1,      SATR,           '-');

    scanner.add_transition(S0,          SOPR_3,         '>');
    scanner.add_transition(SOPR_3,      SOPR_4,         '=');
    scanner.add_transition(S0,          SOPR_4,         '=');

	//literais
    scanner.add_transition(S0,          SL_0,           '"');
	scanner.add_transition(SL_0,    	SL_0,           scanner.wild_card({'"'}));
	scanner.add_transition(SL_0,        SL_1,           '"');

	scanner.add_transition(S0,        	SEOF,           EOF);

	//comentarios
	scanner.add_transition(S0,        	SC_0,           '{');
	scanner.add_transition(SC_0,       	SC_0,           scanner.wild_card({'}'}));
	scanner.add_transition(SC_0,       	SC_1,           '}');
	//espaços em branco
	scanner.add_transition(S0,			SB,				{' ','\t','\n'});
	scanner.add_transition(SB,			SB,				{' ','\t','\n'});

	//identificadores
	scanner.add_transition(S0,        	SID,           	LETRAS);
	scanner.add_transition(SID,        	SID,           	LETRAS);
	scanner.add_transition(SID,        	SID,           	DIGITOS);
	scanner.add_transition(SID,        	SID,           	'_');

	//numeros
	scanner.add_transition(S0,        	SN_0,          	DIGITOS);
	scanner.add_transition(SN_0,       	SN_0,          	DIGITOS);
	scanner.add_transition(SN_0,       	SN_1,          	'.');
	scanner.add_transition(SN_1,       	SN_1_2,        	DIGITOS);
	scanner.add_transition(SN_1_2,     	SN_1_2,        	DIGITOS);

	scanner.add_transition(SN_1_2,     	SN_2,        	{'E','e'});
	scanner.add_transition(SN_0,       	SN_2,          	{'E','e'});
	scanner.add_transition(SN_2,     	SN_3,        	{'+','-'});
	scanner.add_transition(SN_2,     	SN_4,        	DIGITOS);
	scanner.add_transition(SN_3,     	SN_4,        	DIGITOS);
	scanner.add_transition(SN_4,     	SN_4,        	DIGITOS);


    /********* configurando estados finais, e vinculando com seus respectivos atributos/tokens *********/

	scanner.add_final_state(SL_1,	{Tokens::Literal, Token_types::unknow});	//literais
	scanner.add_final_state(SID, 	{Tokens::id, Token_types::unknow}, true);	//identificador
	scanner.add_final_state(SATR,	{Tokens::RCB, Token_types::unknow});		//atribuição

	//operadores relacionais
	scanner.add_final_state(SOPA,	{Tokens::OPM, Token_types::unknow});	
	scanner.add_final_state(SOPR_1, {Tokens::OPR, Token_types::unknow});
	scanner.add_final_state(SOPR_2, {Tokens::OPR, Token_types::unknow});
	scanner.add_final_state(SOPR_3, {Tokens::OPR, Token_types::unknow});
	scanner.add_final_state(SOPR_4, {Tokens::OPR, Token_types::unknow});
	
	//parenteses
	scanner.add_final_state(SPAR_0,	{Tokens::AB_P, Token_types::unknow});
	scanner.add_final_state(SPAR_1,	{Tokens::FC_P, Token_types::unknow});

	scanner.add_final_state(SPV,	{Tokens::PT_V, Token_types::unknow});			//ponto e vírgula
	scanner.add_final_state(SC_1,	{Tokens::Comentario, Token_types::unknow});		//comentário
	scanner.add_final_state(SB,		{Tokens::Espaco, Token_types::unknow});		//comentário
	scanner.ignore_final_state(SC_1);
	scanner.ignore_final_state(SB);
	
	//numeros
	scanner.add_final_state(SN_0,	{Tokens::Num, Token_types::Inteiro});
	scanner.add_final_state(SN_1_2,	{Tokens::Num, Token_types::Real});
	scanner.add_final_state(SN_4,	{Tokens::Num, Token_types::SCI_NUM});

	//EOF
	scanner.add_final_state(SEOF,	{Tokens::EOF_t, Token_types::unknow});
}