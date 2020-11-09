#ifndef SYNTAX_ANALIZER_HEADER
#define SYNTAX_ANALIZER_HEADER

#include <iostream>
#include <stack>

#include "../LexicalAnalizer.h"
#include "../helper.h"
#include "ActionTable.h"
#include "GotoTable.h"
#include "GrammarRule.h"
#include "enums.h"

using namespace std;

typedef unordered_set<Token> Follow;

class SyntaxAnalizer {
   private:
	LexicalAnalizer& scanner;

	stack<int> pilha;
	ActionTable actionTable;
	GotoTable gotoTable;
	vector<GrammarRule> grammarRules;
	unordered_map<NonTerminalSimbol, Follow> follows;

	void panic_mode_routine(Token_attributes* current_simbol);
	string format_error_message(int errorCode, const Token_attributes &foundToken);

   public:
	unordered_map<int, string> errorCode_messages;

	SyntaxAnalizer(vector<GrammarRule> grammarRules, string actionsTable_csv, string gotoTable_csv,
				   LexicalAnalizer& scanner);
	void analyze();

	inline void set_follow(NonTerminalSimbol nonTerminal, Follow follow) { follows[nonTerminal] = follow; }

	inline ActionTable get_actionTable() { return actionTable; }
	inline GotoTable get_gotoTable() { return gotoTable; }
};

#endif