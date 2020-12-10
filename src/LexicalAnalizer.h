#ifndef LEXICAL_ANALIZER_HEADER
#define LEXICAL_ANALIZER_HEADER

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Automata.h"
#include "utils.h"
#include "enums.h"
#include "semanticAnalysis/SemanticAnalysis.h"

#include <sstream>
#include <string>

using namespace std;

typedef unordered_set<GrammarSimbol_attributes *, GrammarSimbol_attr_ptr_hash, GrammarSimbol_attr_ptr_eq> SimbolsTable;

class LexicalAnalizer {
   private:
	GrammarSimbol_attributes *current_simbol = new GrammarSimbol_attributes();

	Automata automata;
	unordered_set<int> id_final_states;
	unordered_set<int> ignored_final_states;
	unordered_map<int, pair<Token, Token_type>> final_states_token_attr;

	istream& text_stream;
	unsigned int line_count = 1;
	unsigned int column_count = 1;

	// verifica se o estado é de identificador
	inline bool is_id_state(int state) { return id_final_states.count(state); }

	inline bool is_ignored_final_state(int state) { return ignored_final_states.count(state); }

	inline void count_line_column(char c) {
		if (c == '\n') {
			column_count = 1;
			line_count++;
		} else
			column_count++;
	}

   public:
	stringstream error_s;
	SimbolsTable simbols_table;

	LexicalAnalizer(const LexicalAnalizer & orig);

	LexicalAnalizer(istream& text_source, int initial_state);

	LexicalAnalizer(istream& text_source, int initial_state, int rejection_state);

	~LexicalAnalizer(){ delete current_simbol; }

	void add_transition(int src, int dest, char c);

	/** cria uma transicao do estado src para dest, para cada item em list_of_chars*/
	void add_transition(int src, int dest, const vector<char>& list_of_chars);

	void add_final_state(int state, pair<Token, Token_type> state_token_attributes, bool is_id_indicator = false);

	void ignore_final_state(int state);

	GrammarSimbol_attributes& analyze_next();

	inline unsigned int current_line() { return line_count; }
	inline unsigned int current_colum() { return column_count; }
	inline void rewind() {
		text_stream.clear(); text_stream.seekg(0); error_s.clear();
		line_count = column_count = 1;
		vector<GrammarSimbol_attributes *> identifiers;
		for (auto& id : simbols_table)
			if (id->token.simbol.asTerminal == Token::id) identifiers.push_back(id);

		for (auto& id : identifiers) simbols_table.erase(id);
	}

	static vector<char> wild_card(const vector<char>& ignore);
};

#endif