#ifndef AUTOMATA_HEADER
#define AUTOMATA_HEADER

#include <unordered_map>
#include <unordered_set>

#include "utils.h"

class Automata {
   private:
	std::unordered_map<std::pair<int, char>, int, hash_pair> transitions;
	std::unordered_set<int> final_states;
	int s0;			 // initial state
	int rej_s = -1;	 // rejection state

   public:
	Automata(int start);
	Automata(int start, int rejection);

	void add_transition(int i, int j, char c);

	void add_final_state(int state);

	int process_char(int s, char c);

	int process_string(const std::string &str);

	bool is_final_state(int state);

	inline int initial_state() { return s0; }
	inline int rejection_state() { return rej_s; }
};

#endif