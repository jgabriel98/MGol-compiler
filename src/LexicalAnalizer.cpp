#include "LexicalAnalizer.h"
#include <algorithm>
#include <iostream>
#include <string>

#include "color.h"


vector<char> LexicalAnalizer::wild_card(const vector<char> &ignore = {}){
    char inicio_alfabeto = numeric_limits<char>::min();
    char fim_alfabeto = numeric_limits<char>::max();
    
    vector<char> wild_card = vector<char>();
    wild_card.reserve(fim_alfabeto - inicio_alfabeto);

    for(char c=inicio_alfabeto; c < fim_alfabeto; c++){
        bool must_ignore = find(ignore.begin(), ignore.end(), c) != ignore.end();
		if(!must_ignore)
		    wild_card.push_back(c);
	}

    return wild_card;
}


LexicalAnalizer::LexicalAnalizer(istream &text_source, int initial_state): automata(initial_state), text_stream(text_source) {
    final_states_token_attr = unordered_map<int, pair<Token, Token_type>>();
}

LexicalAnalizer::LexicalAnalizer(istream &text_source, int initial_state, int rejection_state): automata(initial_state, rejection_state), text_stream(text_source) {
    final_states_token_attr = unordered_map<int, pair<Token, Token_type>>();
}

void LexicalAnalizer::add_final_state(int state, pair<Token, Token_type> state_token_attributes, bool is_id_indicator) {
    final_states_token_attr[state] = state_token_attributes;
    automata.add_final_state(state);
    if(is_id_indicator)
        id_final_states.insert(state);
}

void LexicalAnalizer::ignore_final_state(int state) {
    if(automata.is_final_state(state) == false)
        throw new logic_error("Não foi possível ignorar o estado final '"+to_string(state)+"': não há registro deste estado final");
    ignored_final_states.insert(state);
}

void LexicalAnalizer::add_transition(int src, int dest, char c) {
    automata.add_transition(src, dest, c);
}

void LexicalAnalizer::add_transition(int src, int dest, const vector<char> &list_of_chars) {
    for(auto c: list_of_chars)
        automata.add_transition(src, dest, c);
}

Token_attributes LexicalAnalizer::analyze_next() {
    char c;
    int new_state, state = automata.initial_state();
    Token_attributes token_attr = {};
    
    while (true) {
        c = text_stream.peek(); text_stream.get();
        new_state = automata.process_char(state, c);
        token_attr.lexema.push_back(c);

        if(new_state == automata.rejection_state()) {       //parou num estado de rejeição pq deu erro ou pq terminou de aceitar o lexema
            if(automata.is_final_state(state)) {	//terminou de aceitar um lexema
				text_stream.unget();

                if(is_ignored_final_state(state)) {
                    token_attr.lexema.clear();
                    state = automata.initial_state();
                    continue;                
                } else {
                    token_attr.lexema.pop_back();
					token_attr.token = final_states_token_attr[state].first;
					token_attr.tipo = final_states_token_attr[state].second;

                    if(is_id_state(state))
                        token_attr = *(simbols_table.insert(token_attr).first);

                }

            } else {    //deu erro mesmo
                token_attr.token = Token::ERRO;
                token_attr.tipo = Token_type::unknow;

                error_s.clear();
                error_s << "Erro ao processar o lexema " << SetBOLD << SetForeYEL << token_attr.lexema << RESETTEXT << ": o caractere " << SetForeRED<<SetBOLD<< c << RESETTEXT 
                        << " não foi reconhecido na linha " << line_count << " e coluna " << column_count << '\0';
            }
            
            break;
        } else {
            count_line_column(c);
        }

        state = new_state;
    }

    return token_attr;
}