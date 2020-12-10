#include "LexicalAnalizer.h"
#include "helper.h"
#include "syntaxAnalysis/SyntaxAnalizer.h"
#include "semanticAnalysis/SemanticAnalysis.h"
#include "semanticAnalysis/MySemanticRulesList.h"


#include <stdio.h>
//#include <sstream>		//provavelmente nao esta usando, necessário apenas para usar a classe istringstream
#include <fstream>
#include <iostream>
#include "color.h"

// clang-format off
enum Estados{ S0=0, SPV, SPAR_0, SPAR_1, SOPR_1, SOPR_2, SOPR_3, SOPR_4, SOPR_5, SOPR_6, SATR, SOPA_0, SOPA_1, SOPA_2, SOPA_3, SL_0, SL_1, SEOF, SC_0, SC_1, SID, SN_0, SN_1, SN_2, SN_1_2, SN_3, SN_4, SB,
			  REJECT = -1
};

const vector<char> DIGITOS = {'0','1','2','3','4','5','6','7','8','9'};
const vector<char> LETRAS = {
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'
};
// clang-format on

void configura_analisador_lexico(LexicalAnalizer& lexical_analizer);
void preenche_tabela_de_simbolos(LexicalAnalizer& lexical_analizer);

GrammarRule parse_grammarRule(string grammarRule);

vector<GrammarRule> le_regras_gramaticais(const string& grammar_file_name);
void le_conjuntos_follow(const string &follows_file_name, SyntaxAnalizer &parser);
void preenche_mensagens_dos_codigos_de_erro(SyntaxAnalizer &parser);

void configura_regras_semanticas(SyntaxAnalizer &parser);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		cout << "Missing source and output file names\n";
		exit(1);
	}
	if (argc < 3) {
		cout << "Missing output file name\n";
		exit(1);
	}
	string sourceFile = string(argv[1]);
	string outputFileName = string(argv[2]);


	// istringstream istr("inicio\n\tvarinicio\nA<-2+4;");
	std::ifstream istr(sourceFile);
	if (!istr) throw runtime_error("Could not open file " + sourceFile);

	LexicalAnalizer scanner = LexicalAnalizer(istr, S0, REJECT);
	configura_analisador_lexico(scanner);
	preenche_tabela_de_simbolos(scanner);

	auto grammarRules = le_regras_gramaticais("resources/grammar.txt");
	SyntaxAnalizer parser = SyntaxAnalizer(grammarRules, "resources/tabela_action.csv", "resources/tabela_goto.csv", scanner); 
	le_conjuntos_follow("resources/conjuntos_follow.txt", parser);
	preenche_mensagens_dos_codigos_de_erro(parser);

	FILE *C_file = fopen(outputFileName.c_str(), "w");
	MySemanticRulesList::configure(C_file, &scanner.simbols_table);

	//############ TRABALHO T1 - ANALISADOR LÉXICO ############
	GrammarSimbol_attributes *token;

	printf("%-20s | %10s | %10s | %s\n", "lexema", "token", "tipo", "erro");
	printf("==================== | ========== | ========== | =======\n");
	do {
		token = &scanner.analyze_next();
		full_print_token_attributes(*token, scanner.error_s, true);	// imprimindo token
	} while (token->token.simbol.asTerminal != Token::EOF_t);

	//############ TRABALHO T2 - ANALISADOR SINTATICO ############
	scanner.rewind();
	parser.analyze();

	//############ TRABALHO T3 - ANALISADOR SEMANTICO ############
	parser.rewind();
	configura_regras_semanticas(parser);
	
	parser.analyze();
	
	
	fclose(C_file);
	istr.close();
	return 0;
}


void configura_regras_semanticas(SyntaxAnalizer& parser) {
	parser.set_semanticRule_callback(parse_grammarRule("P' -> P"), MySemanticRulesList::rule_1);	 // 1
	parser.set_semanticRule_callback(parse_grammarRule("P -> inicio V A"), MySemanticRulesList::rule_2);	 // 2

	parser.set_semanticRule_callback(parse_grammarRule("LV -> varfim ;"), MySemanticRulesList::rule_5);	 // 5
	parser.set_semanticRule_callback(parse_grammarRule("D -> id TIPO ;"), MySemanticRulesList::rule_6);	 // 6

	parser.set_semanticRule_callback(parse_grammarRule("TIPO -> inteiro"), MySemanticRulesList::var_declaration_rule);  // 7
	parser.set_semanticRule_callback(parse_grammarRule("TIPO -> real"), MySemanticRulesList::var_declaration_rule);	 // 8
	parser.set_semanticRule_callback(parse_grammarRule("TIPO -> lit"), MySemanticRulesList::var_declaration_rule);	// 9

	parser.set_semanticRule_callback(parse_grammarRule("ES -> leia id ;"), MySemanticRulesList::rule_11);  // 11

	parser.set_semanticRule_callback(parse_grammarRule("ES -> escreva ARG ;"), MySemanticRulesList::rule_12);  // 12

	parser.set_semanticRule_callback(parse_grammarRule("ARG -> literal"), MySemanticRulesList::rule_13);  // 13

	parser.set_semanticRule_callback(parse_grammarRule("ARG -> num"), MySemanticRulesList::rule_14);  // 14

	parser.set_semanticRule_callback(parse_grammarRule("ARG -> id"), MySemanticRulesList::rule_15);	 // 15

	parser.set_semanticRule_callback(parse_grammarRule("CMD -> id rcb LD ;"), MySemanticRulesList::rule_17);
	parser.set_semanticRule_callback(parse_grammarRule("LD -> OPRD opm OPRD"), MySemanticRulesList::rule_18);
	parser.set_semanticRule_callback(parse_grammarRule("LD -> OPRD"), MySemanticRulesList::rule_19);
	parser.set_semanticRule_callback(parse_grammarRule("OPRD -> id"), MySemanticRulesList::rule_20);
	parser.set_semanticRule_callback(parse_grammarRule("OPRD -> num"), MySemanticRulesList::rule_21);
	parser.set_semanticRule_callback(parse_grammarRule("COND -> CABECALHO CORPO"), MySemanticRulesList::rule_23);
	parser.set_semanticRule_callback(parse_grammarRule("CABECALHO -> se ( EXP_R ) entao"), MySemanticRulesList::rule_24);
	parser.set_semanticRule_callback(parse_grammarRule("EXP_R -> OPRD opr OPRD"), MySemanticRulesList::rule_25);

	parser.set_semanticRule_callback(parse_grammarRule("A -> fim"), MySemanticRulesList::rule_30);
}

vector<GrammarRule> le_regras_gramaticais(const string& grammar_file_name) {
	vector<GrammarRule> grammarRules;
	string line;

	std::ifstream grammar_file(grammar_file_name);
	if (!grammar_file) throw runtime_error("Could not open file " + grammar_file_name);

	while (getline(grammar_file, line)) {
		grammarRules.push_back(parse_grammarRule(line));
	}
	grammar_file.close();
	return grammarRules;
}

GrammarRule parse_grammarRule(string grammarRule) {
	std::istringstream iss(grammarRule);
	GrammarRule rule;
	string a;

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
	return rule;
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

void preenche_mensagens_dos_codigos_de_erro(SyntaxAnalizer &parser) {
	parser.errorCode_messages[0]  = "Esperava 'inicio', mas encontrou '%s'";
	parser.errorCode_messages[1]  = "Encontrou '%s' depois de ler 'fim'";
	parser.errorCode_messages[2]  = "Esperava abertura de bloco com 'varinicio', mas encontrou '%s'";
	parser.errorCode_messages[3]  = "Esperava inicio de codigo de atribuição, condicional ou leia/escreva, mas encontrou '%s'" ;
	parser.errorCode_messages[9]  = "Esperava identificador de uma variável, mas encontrou '%s'";
	parser.errorCode_messages[10] = "Esperava um número, literal ou variável, mas encontrou '%s'";
	parser.errorCode_messages[11] = "Esperava um ';', mas econtrou '%s'";
	parser.errorCode_messages[15] = "Esperava atribuição, mas econtrou '%s'";
	parser.errorCode_messages[16] = "Esperava um comando ou 'fimse', mas encontrou '%s'";
	parser.errorCode_messages[17] = "Esperava (, mas encontrou '%s'";
	parser.errorCode_messages[21] = "Esperava uma variável ou número à direita de atribuição, mas encontrou '%s'";
	parser.errorCode_messages[35] = "Esperava declaração de variável, mas encontrou '%s'";
	parser.errorCode_messages[41] = "Esperava tipo da variável (inteiro, real ou lit), mas encontrou '%s'";
	parser.errorCode_messages[48] = "Esperava numeros ou variáveis na condicional, mas encontrou '%s'";
	parser.errorCode_messages[49] = "Esperava ), mas encontrou '%s'";
	parser.errorCode_messages[50] = "Esperava 'entao', mas encontrou '%s'";
	parser.errorCode_messages[52] = "Esperava um operador relacional, mas encontrou '%s'";
	parser.errorCode_messages[57] = "Esperava numeros ou variáveis na operação aritmética, mas encontrou '%s'";
}

void preenche_tabela_de_simbolos(LexicalAnalizer& lexical_analizer) {
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::inicio, Token_type::unknow, "inicio"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::varinicio, Token_type::unknow, "varinicio"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::varfim, Token_type::unknow, "varfim"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::escreva, Token_type::unknow, "escreva"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::leia, Token_type::unknow, "leia"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::se, Token_type::unknow, "se"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::entao, Token_type::unknow, "entao"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::fimse, Token_type::unknow, "fimse"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::fim, Token_type::unknow, "fim"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::inteiro, Token_type::Inteiro, "inteiro"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::lit, Token_type::Literal, "lit"});
	lexical_analizer.simbols_table.insert(new GrammarSimbol_attributes{Token::real, Token_type::Real, "real"});
}

void configura_analisador_lexico(LexicalAnalizer& scanner) {
	/********** Criando trasições de estados **********/
	// parenteses
	scanner.add_transition(S0, SPAR_0, '(');
	scanner.add_transition(S0, SPAR_1, ')');

	scanner.add_transition(S0, SPV, ';');
	// operadores aritmeticos
	scanner.add_transition(S0, SOPA_0, '+');
	scanner.add_transition(S0, SOPA_1, '-');
	scanner.add_transition(S0, SOPA_2, '*');
	scanner.add_transition(S0, SOPA_3, '/');

	// operadores relacionais, e atribuição (SATR)
	scanner.add_transition(S0, SOPR_1, '<');
	scanner.add_transition(SOPR_1, SOPR_2, '=');
	scanner.add_transition(SOPR_1, SOPR_5, '>');
	scanner.add_transition(SOPR_1, SATR, '-');

	scanner.add_transition(S0, SOPR_3, '>');
	scanner.add_transition(SOPR_3, SOPR_4, '=');
	scanner.add_transition(S0, SOPR_6, '=');

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

	scanner.add_final_state(SL_1, {Token::literal, Token_type::Literal});  // literais
	scanner.add_final_state(SID, {Token::id, Token_type::unknow}, true);  // identificador
	scanner.add_final_state(SATR, {Token::rcb, Token_type::atr});	  // atribuição

	// operadores relacionais
	scanner.add_final_state(SOPA_0, {Token::opm, Token_type::opm_soma});
	scanner.add_final_state(SOPA_1, {Token::opm, Token_type::opm_sub});
	scanner.add_final_state(SOPA_2, {Token::opm, Token_type::opm_mult});
	scanner.add_final_state(SOPA_3, {Token::opm, Token_type::opm_div});
	scanner.add_final_state(SOPR_1, {Token::opr, Token_type::opr_less});
	scanner.add_final_state(SOPR_2, {Token::opr, Token_type::opr_less_eq});
	scanner.add_final_state(SOPR_3, {Token::opr, Token_type::opr_greater});
	scanner.add_final_state(SOPR_4, {Token::opr, Token_type::opr_greater_eq});
	scanner.add_final_state(SOPR_5, {Token::opr, Token_type::opr_neq});
	scanner.add_final_state(SOPR_6, {Token::opr, Token_type::opr_eq});

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
	scanner.add_final_state(SN_4, {Token::num, Token_type::Cientifico});

	// EOF
	scanner.add_final_state(SEOF, {Token::EOF_t, Token_type::unknow});
}