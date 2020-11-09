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
	/* retorna estado -1 quando não tem valor na tabela */
	int get(int state, NonTerminalSimbol simbol);

	/*Na linha do estado indicado, procura por um não-terminal que possui um goto.
	 * retorna: o não terminal encontrado
	 */
	NonTerminalSimbol find_not_empty(int state);
};

#endif