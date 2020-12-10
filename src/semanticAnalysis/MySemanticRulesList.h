
#ifndef SEMANTIC_RULES_LIST_HEADER
#define SEMANTIC_RULES_LIST_HEADER

#define TX_Vars_Delimiter 		"/*----Variaveis temporarias----*/"
#define TX_Vars_Delimiter_End 	"/*-----------------------------*/"

#include <vector>
#include <sstream>

#include "../lexicalAnalysis/LexicalAnalizer.h"
#include "SemanticAnalysis.h"

using namespace std;

class MySemanticRulesList {
   private:
	static FILE *C_code_file;
	static char buff[512];
	static stringstream C_code_stream;

	static SimbolsTable *simbolsTable;
	static unsigned int T_counter;
	static stringstream Tx_stream;
	static unsigned char identation;

	static void create_Tx(GrammarSimbol_attributes *simbol, Token_type type);

   public:

	static void configure(FILE *C_code_output_file, SimbolsTable *simbolsTable);

	static GrammarSimbol_attributes rule_1(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
		string C_code = C_code_stream.str();
		
		int tempVars_start = C_code.find(TX_Vars_Delimiter) + sizeof TX_Vars_Delimiter -1;
		C_code.insert(tempVars_start, '\n'+Tx_stream.str());
		
		fprintf(C_code_file, "%s", C_code.c_str());
		return {NonTerminalSimbol::P_};
	}
	static GrammarSimbol_attributes rule_2(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_5(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_6(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes var_declaration_rule(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_11(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_12(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_13(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_14(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_15(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_17(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_18(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_19(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_20(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_21(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_23(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);
	
	static GrammarSimbol_attributes rule_24(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_25(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	static GrammarSimbol_attributes rule_30(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c);

	
};

#endif