#ifndef SYNTAX_ANALIZER_GRAMMAR_RULE_HEADER
#define SYNTAX_ANALIZER_GRAMMAR_RULE_HEADER

#include "../LexicalAnalizer.h"
#include "../helper.h"
#include "enums.h"

class GrammarRule {
   private:
	union _grammarSimbol {
		Token terminal;
		NonTerminalSimbol nonTerminal;
	};
	struct GrammarSimbol {
		union _grammarSimbol simbol;
		bool isTerminal;

		inline operator std::string() const {
			return isTerminal ? Token_to_string(simbol.terminal) : NonTerminalSimbol_to_string(simbol.nonTerminal);
		}
	};

	pair<NonTerminalSimbol, vector<GrammarSimbol>> grammar_rule;

   public:
	void left(NonTerminalSimbol simbol);
	void add_right(Token terminal);
	void add_right(NonTerminalSimbol nonTerminal);

	NonTerminalSimbol left();
	vector<GrammarSimbol> right();

	operator std::string();
};

#endif