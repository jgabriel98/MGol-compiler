#include "SyntaxAnalizer.h"

#include "../color.h"

SyntaxAnalizer::SyntaxAnalizer(vector<GrammarRule> grammarRules,
							   string actionsTable_csv,
							   string gotoTable_csv,
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
		if (nonTerminal)
			break;
		pilha.pop();
	}

	while (current_simbol->token != EOF_t) {
		if (follows.at(nonTerminal).count(current_simbol->token))  // se o terminal percence ao conjunto follow
			break;

		cout << LGTBLK("\t-modo panico-") " descartando o token <" << Token_to_string(current_simbol->token) << ", "
			 << FOREYEL << current_simbol->lexema << RESETTEXT << '>' << endl;
		*current_simbol = scanner.analyze_next();
	}

	pilha.push(gotoTable.get(estado, nonTerminal));
}

void SyntaxAnalizer::analyze() {
	Token_attributes simbolo = scanner.analyze_next();
	while (true) {
		int topo = pilha.top();

		auto table_value = actionTable.get(topo, simbolo.token);
		Action action_type = table_value.first;
		int goto_or_grammarRule = table_value.second;

		if (action_type == Action::Shift) {
			pilha.push(goto_or_grammarRule);
			simbolo = scanner.analyze_next();
		} else if (action_type == Action::Reduce) {
			auto& rule = grammarRules[goto_or_grammarRule - 1];
			for (auto& each : rule.right())
				pilha.pop();

			topo = pilha.top();
			pilha.push(gotoTable.get(topo, rule.left()));

			cout << string(rule) << endl;
		} else if (action_type == Action::Accept) {
			cout << BOLD(LGTGRN("Aceitou")) << endl;
			break;
		} else {
			if (scanner.error_s.peek() != EOF) {  // se for erro leixco
				string error;
				std::getline(scanner.error_s, error);
				cout << RED("Erro léxico: ") << error << endl;
			}
			cout << RED("Erro sintático:") " esperava um 'sla oq', mas encontrou '" << SetBOLD << FORELGTYEL << simbolo.lexema
				 << RESETTEXT << "' na linha " << scanner.current_line() << " e coluna " << scanner.current_colum()
				 << endl;

			panic_mode_routine(&simbolo);

			if (simbolo.token == Token::EOF_t)
				break;
		}
	}
}