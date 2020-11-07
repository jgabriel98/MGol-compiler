#ifndef SYNTAX_ANALIZER_GOTOTABLE_HEADER
#define SYNTAX_ANALIZER_GOTOTABLE_HEADER

#include <string>
#include <unordered_map>
#include <vector>

#include "enums.h"

using namespace std;

class GotoTable {
	vector<vector<string>> table;
	unordered_map<NonTerminalSimbol, int> simbol_to_columIdx;

   public:
	GotoTable(string table_csv);
	int get(int state, NonTerminalSimbol simbol);
};

#endif