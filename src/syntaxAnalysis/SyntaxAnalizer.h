#ifndef SYNTAX_ANALIZER_HEADER
#define SYNTAX_ANALIZER_HEADER

#include <iostream>
#include <stack>

#include "../helper.h"
#include "ActionTable.h"
#include "GotoTable.h"
#include "LexicalAnalizer.h"
#include "enums.h"

using namespace std;

class GrammarRule {
   private:
	union _grammarSimbol {
		Token terminal;
		NonTerminalSimbol nonTerminal;
	};
	struct GrammarSimbol {
		union _grammarSimbol simbol;
		bool isTerminal;

		operator std::string() const {
			return isTerminal ? Token_to_string(simbol.terminal) : NonTerminalSimbol_to_string(simbol.nonTerminal);
		}
	};

	pair<NonTerminalSimbol, vector<GrammarSimbol>> grammar_rule;

   public:
	void set_left(NonTerminalSimbol simbol) { grammar_rule.first = simbol; }
	void add_right(Token terminal) {
		GrammarSimbol simbol = {terminal, true};
		grammar_rule.second.push_back(simbol);
	}
	void add_right(NonTerminalSimbol nonTerminal) {
		GrammarSimbol simbol;
		simbol.simbol.nonTerminal = nonTerminal;
		simbol.isTerminal = false;
		grammar_rule.second.push_back(simbol);
	}

	NonTerminalSimbol left() { return grammar_rule.first; }

	vector<GrammarSimbol> right() { return grammar_rule.second; }

	operator std::string() {
		std::stringstream ss;
		ss << NonTerminalSimbol_to_string(left()) << "->";
		for (auto& simbol : grammar_rule.second)
			ss << ' ' << string(simbol);
	}
};

class SyntaxAnalizer {
   private:
	LexicalAnalizer& scanner;

	vector<GrammarRule> grammarRules;
	ActionTable actionTable;
	GotoTable gotoTable;
	stack<int> pilha;

	void error_routine();

   public:
	SyntaxAnalizer(vector<GrammarRule> grammerRules,
				   string terminalsTable_csv,
				   string nonTerminalsTable_csv,
				   LexicalAnalizer& scanner);
	void analyze();
};

SyntaxAnalizer::SyntaxAnalizer(vector<GrammarRule> grammarRules,
							   string terminalsTable_csv,
							   string nonTerminalsTable_csv,
							   LexicalAnalizer& scanner)
	: scanner(scanner), actionTable(terminalsTable_csv), gotoTable(nonTerminalsTable_csv) {
	this->pilha = stack<int>({0});
}

void SyntaxAnalizer::error_routine(){
	cout<< "DEU MT RUIM";
}

void SyntaxAnalizer::analyze() {
	Token_attributes simbolo = scanner.analyze_next();
	while (true) {
		int topo = pilha.top();

		auto table_value = actionTable.get(topo, simbolo.token);
		Action action = table_value.first;
		int goto_or_grammarNum = table_value.second;

		if (action == Action::Shift) {
			pilha.push(goto_or_grammarNum);
			simbolo = scanner.analyze_next();
		} else if (action == Action::Reduce) {
			auto& rule = grammarRules[goto_or_grammarNum - 1];
			for (auto& each : rule.right())
				pilha.pop();

			topo = pilha.top();
			pilha.push(gotoTable.get(topo, rule.left()));

			cout << string(rule);
		} else if (action == Action::Accept) {
			break;
		} else {
			error_routine();
		}
	}
}

#endif