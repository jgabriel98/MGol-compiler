#ifndef LEXICAL_ANALIZER_HEADER
#define LEXICAL_ANALIZER_HEADER

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Automata.h"
#include "utils.h"

#include <sstream>
#include <string>

using namespace std;

enum Token {
	inicio,
	varinicio,
	varfim,
	escreva,
	leia,
	se,
	entao,
	fimse,
	fim,
	inteiro,
	lit,
	real,  // palavras reservadas
	num,
	literal,
	id,
	EOF_t,
	opr,   // operador relacional
	rcb,   // atribuição
	opm,   // operador aritmetico
	AB_P,  //(
	FC_P,  //)
	PT_V,  //;
	ERRO,
	Comentario,
	Espaco
};

enum Token_type {
	unknow,
	Inteiro,
	Real,
	SCI_NUM	 // notação cientifica
};

struct Token_attributes {
	Token token;
	Token_type tipo;
	string lexema;

	inline bool operator==(const Token_attributes& rhs) const { return lexema == rhs.lexema; }
};

template <>
struct std::hash<Token_attributes> {
	inline std::size_t operator()(const Token_attributes& t) const noexcept {
		return std::hash<std::string>{}(t.lexema);
	}
};

class LexicalAnalizer {
   private:
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
	unordered_set<Token_attributes> simbols_table;

	LexicalAnalizer(istream& text_source, int initial_state);

	LexicalAnalizer(istream& text_source, int initial_state, int rejection_state);

	void add_transition(int src, int dest, char c);

	/** cria uma transicao do estado src para dest, para cada item em list_of_chars*/
	void add_transition(int src, int dest, const vector<char>& list_of_chars);

	void add_final_state(int state, pair<Token, Token_type> state_token_attributes, bool is_id_indicator = false);

	void ignore_final_state(int state);

	Token_attributes analyze_next();

	inline unsigned int current_line() { return line_count; }
	inline unsigned int current_colum() { return column_count; }
	inline void rewind() {
		text_stream.clear(); text_stream.seekg(0);
		line_count = column_count = 1;
	}

	static vector<char> wild_card(const vector<char>& ignore);
};

#endif