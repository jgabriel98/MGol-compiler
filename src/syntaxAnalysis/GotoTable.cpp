#include "GotoTable.h"
#include "CSVReader.h"

using namespace std;

GotoTable::GotoTable(string table_csv) {
	this->table = CSVReader(table_csv).getData();
	auto& header = table[0];
	for (int c = 1; c < header.size(); c++) {
		NonTerminalSimbol t = string_to_NonTerminalSimbol(header[c]);
		simbol_to_columIdx[t] = c;
	}
}

int GotoTable::get(int state, NonTerminalSimbol terminal_simbol) {
	if (simbol_to_columIdx.count(terminal_simbol) == 0)
		return -1;

	int c = simbol_to_columIdx[terminal_simbol];
	string& value = table[state + 1][c];

	if (value.empty())
		return -1;

	// todo: ao inves de converter toda vez, é melhor pre-processar tudo no contrutor.
	int goto_state = stoi(value);
	return goto_state;
}

NonTerminalSimbol GotoTable::find_not_empty(int state) {
	for (int i = NonTerminalSimbol::P_; i <= NonTerminalSimbol::CORPO; i++) {
		auto value = get(state, (NonTerminalSimbol)i);
		if(value != -1){
			return (NonTerminalSimbol)i;
		}
	}
	return {};
}