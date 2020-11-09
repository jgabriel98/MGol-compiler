#ifndef SYNTAX_ANALIZER_HEADER
#define SYNTAX_ANALIZER_HEADER

#include <iostream>
#include <stack>

#include "../LexicalAnalizer.h"
#include "../helper.h"
#include "ActionTable.h"
#include "GotoTable.h"
#include "enums.h"

using namespace std;

class GrammarRule;
typedef unordered_set<Token> Follow;

class SyntaxAnalizer {
   private:
	LexicalAnalizer& scanner;

	vector<GrammarRule> grammarRules;
	unordered_map<NonTerminalSimbol, Follow> follows;
	ActionTable actionTable;
	GotoTable gotoTable;
	stack<int> pilha;

	// retorna: o token de parada responsável pela resincronização. Consequentemente, o ultimo token descartado
	void panic_mode_routine(Token_attributes *current_simbol);

   public:
	SyntaxAnalizer(vector<GrammarRule> grammarRules,
				   string actionsTable_csv,
				   string gotoTable_csv,
				   LexicalAnalizer& scanner);
	void analyze();

	inline void set_follow(NonTerminalSimbol nonTerminal, Follow follow) { follows[nonTerminal] = follow; }

	inline ActionTable get_actionTable() { return actionTable; }
	inline GotoTable get_gotoTable() { return gotoTable; }
};

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
	void left(NonTerminalSimbol simbol) { grammar_rule.first = simbol; }
	void add_right(vector<NonTerminalSimbol> nonTerminal_list) {
		for (auto& item : nonTerminal_list)
			add_right(item);
	}
	void add_right(vector<Token> terminal_list) {
		for (auto& item : terminal_list)
			add_right(item);
	}

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
		ss << NonTerminalSimbol_to_string(left()) << " ->";
		for (auto& simbol : grammar_rule.second)
			ss << ' ' << string(simbol);
		return ss.str();
	}
};

#endif