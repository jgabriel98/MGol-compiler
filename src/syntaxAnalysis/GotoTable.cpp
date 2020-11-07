#include "GotoTable.h"
#include "CSVReader.h"

using namespace std;


GotoTable::GotoTable(string table_csv) {
	this->table = CSVReader(table_csv).getData();
	auto& header = table[0];
	for (int c = 0; c < header.size(); c++) {
		NonTerminalSimbol t = string_to_NonTerminalSimbol(header[c]);
		simbol_to_columIdx[t] = c;
	}
}

// todo: ao inves de converter toda vez, é melhor pre-processar tudo no contrutor.
int GotoTable::get(int state, NonTerminalSimbol terminal_simbol) {
	int c = simbol_to_columIdx[terminal_simbol];
	string& value = table[state][c];

	int goto_state = stoi(value.substr(1));
	return goto_state;
}