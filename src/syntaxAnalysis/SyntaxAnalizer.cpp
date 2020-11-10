#include "SyntaxAnalizer.h"

#include <stdio.h>
#include "../color.h"

SyntaxAnalizer::SyntaxAnalizer(vector<GrammarRule> grammarRules, string actionsTable_csv, string gotoTable_csv,
							   LexicalAnalizer& scanner)
	: grammarRules(grammarRules), scanner(scanner), actionTable(actionsTable_csv), gotoTable(gotoTable_csv) {
	this->pilha = stack<int>({0});
}

void SyntaxAnalizer::panic_mode_routine(Token_attributes* current_simbol) {
	NonTerminalSimbol nonTerminal;
	int estado;
	while (true) {
		estado = pilha.top();
		nonTerminal = gotoTable.find_not_empty(estado);
		if (nonTerminal) break;
		pilha.pop();
	}
	cout<<LGTBLK("\t-modo panico-")<< " procurando pertencente de Follow(" << NonTerminalSimbol_to_string(nonTerminal)<<")" << endl;

	while (current_simbol->token != EOF_t) {
		*current_simbol = scanner.analyze_next();
		if (follows.at(nonTerminal).count(current_simbol->token))  // se o terminal percence ao conjunto follow
			break;

		cout << LGTBLK("\t-modo panico-") " descartou o token <" << Token_to_string(current_simbol->token) << ", "
			 << FOREYEL << current_simbol->lexema << RESETTEXT << '>' << endl;
	}
	cout << LGTBLK("\t-modo panico-") << " sincronizou com o token <" << Token_to_string(current_simbol->token) << ", "
		 << FOREYEL << current_simbol->lexema << RESETTEXT << '>' << endl;

	pilha.push(gotoTable.get(estado, nonTerminal));
}

void SyntaxAnalizer::analyze() {
	unsigned int total_erros = 0;
	Token_attributes simbolo = scanner.analyze_next();
	while (true) {
		int state, ruleNum, errorCode, topo = pilha.top();

		auto table_value = actionTable.get(topo, simbolo.token);
		Action action = table_value.first;
		state = ruleNum = errorCode = table_value.second;

		if (action == Action::Shift) {
			pilha.push(state);
			simbolo = scanner.analyze_next();
		} else if (action == Action::Reduce) {
			auto& rule = grammarRules[ruleNum - 1];
			for (auto& each : rule.right()) pilha.pop(); 

			topo = pilha.top();
			pilha.push(gotoTable.get(topo, rule.left()));

			cout << string(rule) << endl;
		} else if (action == Action::Accept) {
			cout << SetBOLD FORELGTGRN << "Aceitou!" << RESETTEXT << endl;
			break;
		} else {
			total_erros++;
			if (scanner.error_s.peek() != EOF) {  // se for erro lexico
				string error;
				while(std::getline(scanner.error_s, error, '\0'))
					cout << RED("Erro léxico: ") << error << endl;
			}

			cout << RED("Erro sintático: ") << format_error_message(errorCode, simbolo) <<endl;
			if(simbolo.token == Token::EOF_t) break;
			panic_mode_routine(&simbolo);
		}
	}

	cout << "parse completado";
	if (total_erros)
		cout <<" - "<< SetBOLD FORELGTRED << total_erros << " erro(s) foi encontrado" << RESETTEXT << endl;
		
}

string SyntaxAnalizer::format_error_message(int errorCode, const Token_attributes &foundToken){
	char msg_out[256];
	string foundLexem = SetBOLD+(SetForeYEL+foundToken.lexema+RESETTEXT);
	string msg_to_format = errorCode_messages[errorCode];
	if(msg_to_format.empty()){
		msg_to_format = "Símbolo '%s' inválido";
	}
	msg_to_format += ", na linha %d e coluna %d";

	sprintf(msg_out, msg_to_format.c_str(), foundLexem.c_str(), scanner.current_line(), scanner.current_colum());
	return string(msg_out);
}
