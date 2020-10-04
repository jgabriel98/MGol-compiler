#ifndef LEXICAL_ANALIZER_HEADER
#define LEXICAL_ANALIZER_HEADER

#include "Automata.h"
#include "utils.h"
#include <unordered_map>
#include <vector>

#include <sstream>

using namespace std;

enum Tokens {
    ERRO,
    inicio, varinicio, varfim, escreva, leia, se, entao, fimse, fim, inteiro, lit, real,	//palavras reservadas
    Num,
    Literal,
    id,
    Comentario,
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
    //string erro;
};



class LexicalAnalizer {

private:
    Automata automata;
	unordered_set<int> id_final_states;
    unordered_map<int, pair<Tokens, Token_types>> final_states_token_attr;
    unsigned int line_count = 1;
    unsigned int column_count = 1;

    void ignore_white_spaces(istream &text_stream);

public:

    stringstream error_s;
    unordered_map<string, Token_attributes> simbols_table;


    LexicalAnalizer(int initial_state);

    LexicalAnalizer(int initial_state, int rejection_state);

    void add_transition(int src, int dest, char c);

    /** cria uma transicao do estado src para dest, para cada item em list_of_chars*/
    void add_transition(int src, int dest, const vector<char> &list_of_chars);

    void add_final_state(int state, pair<Tokens, Token_types> state_token_attributes, bool is_id_indicator = false);
    
	Token_attributes analyze(istream &text_stream);

    static vector<char> wild_card(const vector<char> &ignore);
};

#endif