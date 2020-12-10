#ifndef SYNTAX_ANALIZER_HEADER
#define SYNTAX_ANALIZER_HEADER


#include <stack>

#include "../lexicalAnalysis/LexicalAnalizer.h"
#include "../helper.h"
#include "ActionTable.h"
#include "GotoTable.h"
#include "GrammarRule.h"
#include "../semanticAnalysis/SemanticAnalysis.h"
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
	
	stack<GrammarSimbol_attributes *> pilha_semantica;
	unordered_map<GrammarRule, semanticRule, GrammarRule::hash_func> semanticRules;

	void panic_mode_routine(GrammarSimbol_attributes* current_simbol);
	string format_error_message(int errorCode, const GrammarSimbol_attributes &foundToken);
	void semantic_step(GrammarRule &rule);

   public:
	unordered_map<int, string> errorCode_messages;

	SyntaxAnalizer(vector<GrammarRule> grammarRules, string actionsTable_csv, string gotoTable_csv,
				   LexicalAnalizer& scanner);
	void analyze();

	/** When the given GrammarRule is reduced, it will call the given semanticRule() callback*/
	void set_semanticRule_callback(GrammarRule grammarRule, semanticRule callback);
	/** When the given GrammarRule of number grammarRule_num is reduced, it will call the given semanticRule() callback*/
	void set_semanticRule_callback(size_t grammarRule_num, semanticRule callback );
	inline void set_follow(NonTerminalSimbol nonTerminal, Follow follow) { follows[nonTerminal] = follow; }

	inline ActionTable get_actionTable() { return actionTable; }
	inline GotoTable get_gotoTable() { return gotoTable; }

	inline void rewind() {
		scanner.rewind();
		pilha = stack<int>({0});
		pilha_semantica = stack<GrammarSimbol_attributes *>();
	}
};

#endif