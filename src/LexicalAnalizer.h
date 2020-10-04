#ifndef LEXICAL_ANALIZER_HEADER
#define LEXICAL_ANALIZER_HEADER

#include "Automata.h"
#include <unordered_map>
#include <vector>

#include <sstream>

using namespace std;

enum Tokens {
    ERRO,
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
    unordered_map<int, pair<Tokens, Token_types>> final_states_token_attr;
    vector<Token_attributes> simbols_table;
    unsigned int line_count = 1;
    unsigned int column_count = 1;

    void ignore_white_spaces(istream &text_stream);

public:

    stringstream error_s;

    LexicalAnalizer(int initial_state);

    LexicalAnalizer(int initial_state, int rejection_state);

    void add_final_state(int state, pair<Tokens, Token_types> state_token_attributes);

    void add_transition(int src, int dest, char c);

    /** cria uma transicao do estado src para dest, para cada item em list_of_chars*/
    void add_transition(int src, int dest, const vector<char> &list_of_chars);

    static vector<char> wild_card(const vector<char> &ignore);

    Token_attributes analyze(istream &text_stream);

};

#endif