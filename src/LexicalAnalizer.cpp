#include "LexicalAnalizer.h"
#include <algorithm>
#include <iostream>
#include <string>



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

void LexicalAnalizer::add_transition(int src, int dest, char c) {
    automata.add_transition(src, dest, c);
}

void LexicalAnalizer::add_transition(int src, int dest, const vector<char> &list_of_chars) {
    for(auto c: list_of_chars)
        automata.add_transition(src, dest, c);
}

void LexicalAnalizer::ignore_white_spaces(istream &text_stream) {
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

Token_attributes LexicalAnalizer::analyze(istream &text_stream) {
    char c;
    int new_state, state = automata.initial_state();
    Token_attributes token_attr = {};

    
    ignore_white_spaces(text_stream);
    
    while (true) {
        c = text_stream.peek(); text_stream.get();
        new_state = automata.process_char(state, c);
        token_attr.lexema.push_back(c);
        column_count++;

        if(new_state == automata.rejection_state()){    //estado de rejeição pq deu erro ou pq terminou de aceitar o lexema
            break;
        }
        
        state = new_state;
    }

    if(automata.is_final_state(state)){
        text_stream.unget();
        token_attr.lexema.pop_back();
        column_count--;

        token_attr.token = final_states_token_attr[state].first;
        token_attr.tipo = final_states_token_attr[state].second;

        if(id_final_states.count(state)){
            simbols_table[token_attr.lexema] = token_attr;
        }

    } else {
        token_attr.token = Tokens::ERRO;
        token_attr.tipo = Token_types::unknow;

        error_s.clear();
        error_s << "O lexema " << token_attr.lexema << " não foi reconhecido, na linha " << line_count << " e coluna " << column_count;
    }

    return token_attr;
}