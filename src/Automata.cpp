#include "Automata.h"
#include <map>
#include <vector>

using namespace std;



Automata::Automata(int start) : s0(start) {}
Automata::Automata(int start, int rejection) : s0(start), rej_s(rejection) {}

void Automata::add_transition(int i, int j, char c) {
	transitions[{i, c}] = j;
}

void Automata::add_final_state(int state) {
	if(state == rej_s){
		throw new logic_error("Um estado final não pode ser o estado de rejeição.");
	}
	final_states.insert(state);
}

bool Automata::is_final_state(int state){
	return final_states.count(state);
}

int Automata::process_char(int s, char c) {
	auto next_state_itr = transitions.find({s, c});

	if(next_state_itr == transitions.end())	//rejeita
		return rej_s;
	
	return next_state_itr->second;
}

int Automata::process_string(const string &str) {
	int current_state = s0;

	for(char c: str){
		current_state = process_char(current_state, c);
		if(current_state == rej_s){	//rejeita
			break;
		}
	}

	return current_state;
}
