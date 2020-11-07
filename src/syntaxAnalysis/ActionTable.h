#ifndef SYNTAX_ANALIZER_ACTIONTABLE_HEADER
#define SYNTAX_ANALIZER_ACTIONTABLE_HEADER

#include <unordered_map>
#include <vector>

#include "../LexicalAnalizer.h"
#include "enums.h"

using namespace std;

class ActionTable {
	vector<vector<string>> table;
	unordered_map<Token, int> terminal_to_columIdx;

   public:
	ActionTable(string table_csv);
	pair<Action, int> get(int state, Token column);
};

#endif