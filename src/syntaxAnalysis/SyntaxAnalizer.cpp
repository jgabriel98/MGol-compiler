#include "SyntaxAnalizer.h"

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include "../color.h"

SyntaxAnalizer::SyntaxAnalizer(vector<GrammarRule> grammarRules, string actionsTable_csv, string gotoTable_csv,
							   LexicalAnalizer& scanner)
	: grammarRules(grammarRules), scanner(scanner), actionTable(actionsTable_csv), gotoTable(gotoTable_csv) {
	this->pilha = stack<int>({0});
}

void SyntaxAnalizer::set_semanticRule_callback(GrammarRule grammarRule, semanticRule callback) {
	auto itr = std::find(grammarRules.begin(), grammarRules.end(), grammarRule);
	if (itr == grammarRules.end()) {
		throw new logic_error("Não foi possível adicionar a regra semantica. A regra gramatical " +
							  string(grammarRule) + " não existe.");
	}
	size_t idx = std::distance(grammarRules.begin(), itr);
	set_semanticRule_callback(idx+1, callback);
}

void SyntaxAnalizer::set_semanticRule_callback(size_t grammarRule_num, semanticRule callback ) {
	semanticRules[grammarRules[grammarRule_num -1 ]] = callback;
}

void SyntaxAnalizer::panic_mode_routine(GrammarSimbol_attributes* current_simbol) {
	NonTerminalSimbol nonTerminal;
	int estado;
	while (true) {
		estado = pilha.top();
		nonTerminal = gotoTable.find_not_empty(estado);
		if (nonTerminal) break;
		pilha.pop();
		pilha_semantica.pop();
	}
	cout<<LGTBLK("\t-modo panico-")<< " procurando pertencente de Follow(" << NonTerminalSimbol_to_string(nonTerminal)<<")" << endl;

	while (current_simbol->token.simbol.asTerminal != EOF_t) {
		*current_simbol = scanner.analyze_next();
		if (follows.at(nonTerminal).count(current_simbol->token.simbol.asTerminal))  // se o terminal percence ao conjunto follow
			break;

		// cout << LGTBLK("\t-modo panico-") " descartou o token <" << Token_to_string(current_simbol->token.simbol.asTerminal) << ", "
		// 	 << FOREYEL << current_simbol->lexema << RESETTEXT << '>' << endl;
	}
	cout << LGTBLK("\t-modo panico-") << " sincronizou com o token <" << Token_to_string(current_simbol->token.simbol.asTerminal) << ", "
		 << FOREYEL << current_simbol->lexema << RESETTEXT << '>' << endl;

	pilha.push(gotoTable.get(estado, nonTerminal));
	pilha_semantica.push(new GrammarSimbol_attributes({nonTerminal}));
}


void SyntaxAnalizer::semantic_step(GrammarRule &rule){
	size_t rule_size = rule.right().size();
	vector<GrammarSimbol_attributes*> right_simbols_attr(rule_size);
	for (int i = rule_size - 1; i >= 0; i--) {
		right_simbols_attr[i] = pilha_semantica.top();
		pilha_semantica.pop();
	}

	if (semanticRules.count(rule)) {
		auto grammar_attributtes = semanticRules[rule](right_simbols_attr, scanner.current_line(), scanner.current_colum());
		pilha_semantica.push(new GrammarSimbol_attributes(grammar_attributtes));
	}else{
		pilha_semantica.push(new GrammarSimbol_attributes({rule.left()}));
	}
/*
	for (auto& old_simbol : right_simbols_attr) {
		if (old_simbol->token.isTerminal == false ||
			(old_simbol->token.isTerminal && old_simbol->token.simbol.asTerminal != Token::id))
			delete old_simbol;
	}*/
}

void SyntaxAnalizer::analyze() {
	unsigned int total_erros = 0;
	GrammarSimbol_attributes *simbolo = &scanner.analyze_next();

	while (true) {
		int state, ruleNum, errorCode, topo = pilha.top();

		auto table_value = actionTable.get(topo, simbolo->token.simbol.asTerminal);
		Action action = table_value.first;
		state = ruleNum = errorCode = table_value.second;

		if (action == Action::Shift) {
			pilha.push(state);
			pilha_semantica.push(simbolo);
			simbolo = &scanner.analyze_next();
		} else if (action == Action::Reduce) {
			auto& rule = grammarRules[ruleNum - 1];
			for (auto& each : rule.right()) pilha.pop();

			topo = pilha.top();
			pilha.push(gotoTable.get(topo, rule.left()));

			cout << string(rule) << endl;

			semantic_step(rule);

		} else if (action == Action::Accept) {
			cout << SetBOLD FORELGTGRN << "Aceitou!" << RESETTEXT << endl;
			auto& rule = grammarRules[0];
			semantic_step(rule);
			break;
		} else {
			total_erros++;
			if (scanner.error_s.peek() != EOF) {  // se for erro lexico
				string error;
				while(std::getline(scanner.error_s, error, '\0'))
					cout << RED("Erro léxico: ") << error << endl;
			}
			cout << RED("Erro sintático: ") << format_error_message(errorCode, *simbolo) <<endl;
			
			if(simbolo->token.simbol.asTerminal == Token::EOF_t) break;
			panic_mode_routine(simbolo);
		}
	}

	cout << "parse completado";
	if (total_erros)
		cout <<" - "<< SetBOLD FORELGTRED << total_erros << " erro(s) foi encontrado" << RESETTEXT << endl;
	cout<<endl;
		
}

string SyntaxAnalizer::format_error_message(int errorCode, const GrammarSimbol_attributes &foundToken){
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
