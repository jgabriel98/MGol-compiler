#include "LexicalAnalizer.h"
#include <algorithm>
#include <iostream>
#include <string>

#include "color.h"


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


LexicalAnalizer::LexicalAnalizer(int initial_state): automata(initial_state) {
    final_states_token_attr = unordered_map<int, pair<Tokens, Token_types>>();
    error_s.clear();
}

LexicalAnalizer::LexicalAnalizer(int initial_state, int rejection_state): automata(initial_state, rejection_state) {
    final_states_token_attr = unordered_map<int, pair<Tokens, Token_types>>();
    error_s.clear();
}

void LexicalAnalizer::add_final_state(int state, pair<Tokens, Token_types> state_token_attributes, bool is_id_indicator) {
    final_states_token_attr[state] = state_token_attributes;
    automata.add_final_state(state);
    if(is_id_indicator)
        id_final_states.insert(state);
}

void LexicalAnalizer::ignore_state(int state) {
    ignored_final_states.insert(state);
}

void LexicalAnalizer::add_transition(int src, int dest, char c) {
    automata.add_transition(src, dest, c);
}

void LexicalAnalizer::add_transition(int src, int dest, const vector<char> &list_of_chars) {
    for(auto c: list_of_chars)
        automata.add_transition(src, dest, c);
}

Token_attributes LexicalAnalizer::analyze(istream &text_stream) {
    char c;
    int new_state, state = automata.initial_state();
    Token_attributes token_attr = {};
    
    while (true) {
        c = text_stream.peek(); text_stream.get();
        new_state = automata.process_char(state, c);
        token_attr.lexema.push_back(c);

        if(new_state == automata.rejection_state()) {       //parou num estado de rejeição pq deu erro ou pq terminou de aceitar o lexema
            if(automata.is_final_state(state)) {
                text_stream.unget();

                if(is_ignored_final_state(state)) {
                    token_attr.lexema.clear();
                    state = automata.initial_state();
                    continue;                
                } else {
                    token_attr.lexema.pop_back();

                    token_attr.token = final_states_token_attr[state].first;
                    token_attr.tipo = final_states_token_attr[state].second;
                    add_token_to_simbols_if_allowed(state, token_attr);
                }

            } else {    //deu erro mesmo
                token_attr.token = Tokens::ERRO;
                token_attr.tipo = Token_types::unknow;

                error_s.clear();
                error_s << "Erro ao processar o lexema " << SetBOLD << token_attr.lexema << RESETTEXT << ": o caractere " << SetForeRED<<SetBOLD<< c << RESETTEXT 
                        << " não foi reconhecido na linha " << line_count << " e coluna " << (column_count);                
            }
            
            break;
        } else {
            count_line_column(c);
        }
        
        state = new_state;
    }

    return token_attr;
}