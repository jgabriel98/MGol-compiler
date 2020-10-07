#ifndef LEXICAL_ANALIZER_HEADER
#define LEXICAL_ANALIZER_HEADER

#include "Automata.h"
#include "utils.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <sstream>
#include <string>

using namespace std;

enum Tokens {
    ERRO,
    inicio, varinicio, varfim, escreva, leia, se, entao, fimse, fim, inteiro, lit, real,	//palavras reservadas
    Num,
    Literal,
    id,
    Comentario,
    Espaco,
    EOF_t,
    OPR,    //operador relacional
    RCB,    //atribuição
    OPM,    //operador aritmetico
    AB_P,   //(
    FC_P,   //)
    PT_V    //;
};

enum Token_types {
    unknow,
    Inteiro,
    Real,
    SCI_NUM //notação cientifica
};

struct Token_attributes
{
    Tokens token;
    Token_types tipo;
    string lexema;

    inline bool operator == (const Token_attributes &rhs) const {
        return lexema == rhs.lexema;
    }

};

template<> struct std::hash<Token_attributes> {
	std::size_t operator()(const Token_attributes& t) const noexcept {
		return std::hash<std::string>{}(t.lexema);
	}
};





class LexicalAnalizer {

private:
    Automata automata;
	unordered_set<int> id_final_states;
    unordered_set<int> ignored_final_states;
    unordered_map<int, pair<Tokens, Token_types>> final_states_token_attr;
    
    unsigned int line_count = 1;
    unsigned int column_count = 1;

	//verifica se o estado é de identificador
    inline bool is_id_state(int state){
        return id_final_states.count(state);
    }

    inline bool is_ignored_final_state(int state){
        return ignored_final_states.count(state);
    }

    inline void count_line_column(char c){
        if(c == '\n') {column_count=1; line_count++;} else column_count++;
    }



public:

    stringstream error_s;
    unordered_set<Token_attributes> simbols_table;


    LexicalAnalizer(int initial_state);

    LexicalAnalizer(int initial_state, int rejection_state);

    void add_transition(int src, int dest, char c);

    /** cria uma transicao do estado src para dest, para cada item em list_of_chars*/
    void add_transition(int src, int dest, const vector<char> &list_of_chars);

    void add_final_state(int state, pair<Tokens, Token_types> state_token_attributes, bool is_id_indicator = false);

    void ignore_final_state(int state);
    
	Token_attributes analyze(istream &text_stream);

    static vector<char> wild_card(const vector<char> &ignore);
};

#endif