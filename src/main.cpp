#include "LexicalAnalizer.h"
#include "helper.h"
#include "syntaxAnalysis/SyntaxAnalizer.h"

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

void configura_analisador_lexico(LexicalAnalizer& lexical_analizer);
void preenche_tabela_de_simbolos(LexicalAnalizer& lexical_analizer);
vector<GrammarRule> le_regras_gramaticais(const string& grammar_file_name);
void le_conjuntos_follow(const string &follows_file_name, SyntaxAnalizer &parser);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Missing source file\n";
		exit(1);
	}
	string sourceFile = string(argv[1]);


	// istringstream istr("inicio\n\tvarinicio\nA<-2+4;");
	std::ifstream istr(sourceFile);
	if (!istr) throw runtime_error("Could not open file " + sourceFile);

	LexicalAnalizer scanner = LexicalAnalizer(istr, S0, REJECT);
	configura_analisador_lexico(scanner);
	preenche_tabela_de_simbolos(scanner);

	auto grammarRules = le_regras_gramaticais("resources/grammar.txt");
	SyntaxAnalizer parser =
		SyntaxAnalizer(grammarRules, "resources/tabela_action.csv", "resources/tabela_goto.csv", scanner);
	le_conjuntos_follow("resources/conjuntos_follow.txt", parser);

	//############ TRABALHO T1 - ANALISADOR LÉXICO ############
	Token_attributes token;

	printf("%-20s | %10s | %10s | %s\n", "lexema", "token", "tipo", "erro");
	printf("==================== | ========== | ========== | =======\n");
	do {
		token = scanner.analyze_next();
		full_print_token_attributes(token, scanner, true);	// imprimindo token
	} while (token.token != Token::EOF_t);
	scanner.rewind();

	//############ TRABALHO T2 - ANALISADOR SINTATICO ############

	parser.analyze();

	istr.close();
	return 0;
}

vector<GrammarRule> le_regras_gramaticais(const string& grammar_file_name) {
	vector<GrammarRule> grammarRules;
	// grammarRules[0].left(NonTerminalSimbol::)

	std::string line, a;
	std::ifstream grammar_file(grammar_file_name);
	if (!grammar_file)
		throw runtime_error("Could not open file " + grammar_file_name);

	while (getline(grammar_file, line)) {
		std::istringstream iss(line);
		GrammarRule rule;
		iss >> a;
		rule.left(string_to_NonTerminalSimbol(a));
		iss >> a;

		while (!iss.eof()) {
			iss >> a;
			try {
				rule.add_right(string_to_NonTerminalSimbol(a));
			} catch (const exception& e1) {
				try {
					rule.add_right(string_to_Token(a));
				} catch (const exception& e2) {
					throw runtime_error(string(e1.what()) + "\nand\n" + e2.what());
				}
			}
		}

		// cout << string(rule) << endl;
		grammarRules.push_back(rule);
	}
	grammar_file.close();
	return grammarRules;
}

void le_conjuntos_follow(const string& follows_file_name, SyntaxAnalizer& parser) {
	std::string line, a;
	std::ifstream follow_file(follows_file_name);
	if (!follow_file)
		throw runtime_error("Could not open file " + follows_file_name);

	while (getline(follow_file, line)) {
		std::istringstream iss(line);
		Follow follow;
		NonTerminalSimbol nonterminal;

		iss >> a;
		nonterminal = string_to_NonTerminalSimbol(a);
		iss >> a;

		while (!iss.eof()) {
			iss >> a;
			follow.insert(string_to_Token(a));
		}
		parser.set_follow(nonterminal, follow);
	}

	follow_file.close();
}

void preenche_tabela_de_simbolos(LexicalAnalizer& lexical_analizer) {
	lexical_analizer.simbols_table.insert({Token::inicio, Token_type::unknow, "inicio"});
	lexical_analizer.simbols_table.insert({Token::varinicio, Token_type::unknow, "varinicio"});
	lexical_analizer.simbols_table.insert({Token::varfim, Token_type::unknow, "varfim"});
	lexical_analizer.simbols_table.insert({Token::escreva, Token_type::unknow, "escreva"});
	lexical_analizer.simbols_table.insert({Token::leia, Token_type::unknow, "leia"});
	lexical_analizer.simbols_table.insert({Token::se, Token_type::unknow, "se"});
	lexical_analizer.simbols_table.insert({Token::entao, Token_type::unknow, "entao"});
	lexical_analizer.simbols_table.insert({Token::fimse, Token_type::unknow, "fimse"});
	lexical_analizer.simbols_table.insert({Token::fim, Token_type::unknow, "fim"});
	lexical_analizer.simbols_table.insert({Token::inteiro, Token_type::unknow, "inteiro"});
	lexical_analizer.simbols_table.insert({Token::lit, Token_type::unknow, "lit"});
	lexical_analizer.simbols_table.insert({Token::real, Token_type::unknow, "real"});
}

void configura_analisador_lexico(LexicalAnalizer& scanner) {
	/********** Criando trasições de estados **********/
	// parenteses
	scanner.add_transition(S0, SPAR_0, '(');
	scanner.add_transition(S0, SPAR_1, ')');

	scanner.add_transition(S0, SPV, ';');
	// operadores aritmeticos
	scanner.add_transition(S0, SOPA, {'+', '-', '*', '/'});

	// operadores relacionais, e atribuição (SATR)
	scanner.add_transition(S0, SOPR_1, '<');
	scanner.add_transition(SOPR_1, SOPR_2, {'=', '>'});
	scanner.add_transition(SOPR_1, SATR, '-');

	scanner.add_transition(S0, SOPR_3, '>');
	scanner.add_transition(SOPR_3, SOPR_4, '=');
	scanner.add_transition(S0, SOPR_4, '=');

	// literais
	scanner.add_transition(S0, SL_0, '"');
	scanner.add_transition(SL_0, SL_0, scanner.wild_card({'"', EOF}));
	scanner.add_transition(SL_0, SL_1, '"');

	scanner.add_transition(S0, SEOF, EOF);

	// comentarios
	scanner.add_transition(S0, SC_0, '{');
	scanner.add_transition(SC_0, SC_0, scanner.wild_card({'}', EOF}));
	scanner.add_transition(SC_0, SC_1, '}');
	// espaços em branco
	scanner.add_transition(S0, SB, {' ', '\t', '\n'});
	scanner.add_transition(SB, SB, {' ', '\t', '\n'});

	// identificadores
	scanner.add_transition(S0, SID, LETRAS);
	scanner.add_transition(SID, SID, LETRAS);
	scanner.add_transition(SID, SID, DIGITOS);
	scanner.add_transition(SID, SID, '_');

	// numeros
	scanner.add_transition(S0, SN_0, DIGITOS);
	scanner.add_transition(SN_0, SN_0, DIGITOS);
	scanner.add_transition(SN_0, SN_1, '.');
	scanner.add_transition(SN_1, SN_1_2, DIGITOS);
	scanner.add_transition(SN_1_2, SN_1_2, DIGITOS);

	scanner.add_transition(SN_1_2, SN_2, {'E', 'e'});
	scanner.add_transition(SN_0, SN_2, {'E', 'e'});
	scanner.add_transition(SN_2, SN_3, {'+', '-'});
	scanner.add_transition(SN_2, SN_4, DIGITOS);
	scanner.add_transition(SN_3, SN_4, DIGITOS);
	scanner.add_transition(SN_4, SN_4, DIGITOS);

	/********* configurando estados finais, e vinculando com seus respectivos atributos/tokens *********/

	scanner.add_final_state(SL_1, {Token::literal, Token_type::unknow});  // literais
	scanner.add_final_state(SID, {Token::id, Token_type::unknow}, true);  // identificador
	scanner.add_final_state(SATR, {Token::rcb, Token_type::unknow});	  // atribuição

	// operadores relacionais
	scanner.add_final_state(SOPA, {Token::opm, Token_type::unknow});
	scanner.add_final_state(SOPR_1, {Token::opr, Token_type::unknow});
	scanner.add_final_state(SOPR_2, {Token::opr, Token_type::unknow});
	scanner.add_final_state(SOPR_3, {Token::opr, Token_type::unknow});
	scanner.add_final_state(SOPR_4, {Token::opr, Token_type::unknow});

	// parenteses
	scanner.add_final_state(SPAR_0, {Token::AB_P, Token_type::unknow});
	scanner.add_final_state(SPAR_1, {Token::FC_P, Token_type::unknow});

	scanner.add_final_state(SPV, {Token::PT_V, Token_type::unknow});		 // ponto e vírgula
	scanner.add_final_state(SC_1, {Token::Comentario, Token_type::unknow});	 // comentário
	scanner.add_final_state(SB, {Token::Espaco, Token_type::unknow});		 // comentário
	scanner.ignore_final_state(SC_1);
	scanner.ignore_final_state(SB);

	// numeros
	scanner.add_final_state(SN_0, {Token::num, Token_type::Inteiro});
	scanner.add_final_state(SN_1_2, {Token::num, Token_type::Real});
	scanner.add_final_state(SN_4, {Token::num, Token_type::SCI_NUM});

	// EOF
	scanner.add_final_state(SEOF, {Token::EOF_t, Token_type::unknow});
}