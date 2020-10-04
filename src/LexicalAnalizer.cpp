#include "LexicalAnalizer.h"
#include <algorithm>
#include <istream>
#include <string>

LexicalAnalizer::LexicalAnalizer(int initial_state): automata(initial_state){
    final_states_token_attr = unordered_map<int, pair<Tokens, Token_types>>();
}

LexicalAnalizer::LexicalAnalizer(int initial_state, int rejection_state): automata(initial_state, rejection_state){
    final_states_token_attr = unordered_map<int, pair<Tokens, Token_types>>();
}

void LexicalAnalizer::add_final_state(int state, pair<Tokens, Token_types> state_token_attributes){
    final_states_token_attr[state] = state_token_attributes;
    automata.add_final_state(state);
}

void LexicalAnalizer::add_transition(int src, int dest, char c){
    automata.add_transition(src, dest, c);
}

void LexicalAnalizer::add_transition(int src, int dest, const vector<char> &list_of_chars){
    for(auto c: list_of_chars)
        automata.add_transition(src, dest, c);
}

vector<char> LexicalAnalizer::wild_card(const vector<char> &ignore = {}){
    char inicio_alfabeto = ' ';
    char fim_alfabeto = '~';
    
    vector<char> wild_card = vector<char>();
    wild_card.reserve(fim_alfabeto - inicio_alfabeto +1);

    for(char c=inicio_alfabeto; c<=fim_alfabeto; c++){
        bool must_ignore = find(ignore.begin(), ignore.end(), c) != ignore.end();
		if(!must_ignore)
		    wild_card.push_back(c);
	}

    return wild_card;
}

void LexicalAnalizer::ignore_white_spaces(istream &text_stream){
    while(true) {
        char c = text_stream.peek(); text_stream.get();
        if(c==' ' || c=='\t') {
            column_count++;
        } else if(c=='\n') {
            column_count = 0;
            line_count++;
        } else {
            break;
        }
    }
    text_stream.unget();
}

Token_attributes LexicalAnalizer::analyze(istream &text_stream){
    char c;
    int new_state, state = automata.initial_state();
    Token_attributes token_attr = {};

    //ignora caracteres brancos
    ignore_white_spaces(text_stream);

    
    while (true) {
        c = text_stream.peek(); text_stream.get();
        new_state = automata.process_char(state, c);
        token_attr.lexema.push_back(c);
        column_count++;

        if(new_state == automata.rejection_state()){    //estado de rejeição pq deu erro ou pq terminou de aceitar o lexema
            if(automata.is_final_state(state)){
                text_stream.unget();
                token_attr.lexema.pop_back();
                column_count--;

                token_attr.token = final_states_token_attr[state].first;
                token_attr.tipo = final_states_token_attr[state].second;
            } else {
                token_attr.token = Tokens::ERRO;
                token_attr.tipo = Token_types::unknow;
                error_s << "O lexema " << token_attr.lexema << " não foi reconhecido, na linha " << line_count << " e coluna " <<column_count;
            }
            break;
        }
        
        state = new_state;
    }

    return token_attr;
}