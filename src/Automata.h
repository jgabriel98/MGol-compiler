#ifndef AUTOMATA_HEADER
#define AUTOMATA_HEADER

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <map>
#include <set>
//#include <utility>

using namespace std;



struct hash_pair { 
    template <class T1, class T2> 
    inline size_t operator()(const pair<T1, T2>& p) const { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
}; 


class Automata {

private:
	unordered_map<pair<int, char>, int, hash_pair> transitions;
	unordered_set<int> final_states;
	int s0; // initial state
	int rej_s = -1;	//rejection state

public:

	Automata(int start);
	Automata(int start, int rejection);

	void add_transition(int i, int j, char c);

	void add_final_state(int state);

	int process_char(int s, char c);

	int process_string(const string &str);

	bool is_final_state(int state);

	inline int initial_state() {return s0;}
	inline int rejection_state() {return rej_s;}
};

#endif