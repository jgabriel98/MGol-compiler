#include "ActionTable.h"
#include "CSVReader.h"

#include "../helper.h"

ActionTable::ActionTable(string table_csv) {
	this->table = CSVReader(table_csv).getData();
	auto& header = table[0];
	for (int c = 1; c < header.size(); c++) {
		Token t = string_to_Token(header[c]);
		terminal_to_columIdx[t] = c;
	}
}

// retorna um pair vazio quando não tem valor na tabela
pair<Action, int> ActionTable::get(int state, Token terminal_simbol) {
	if (terminal_to_columIdx.count(terminal_simbol) == 0)
		return {};
		
	int c = terminal_to_columIdx[terminal_simbol];
	string& value = table[state + 1][c];

	if (value.empty()) return {};

	// todo: ao inves de converter toda vez, é melhor pre-processar tudo no contrutor.
	if (value[0] == 'A' || value[0] == 'a')
		return {Action::Accept, -1};

	Action action;
	if (value[0] == 'R' || value[0] == 'r')
		action = Action::Reduce;
	if (value[0] == 's' || value[0] == 'S')
		action = Action::Shift;

	int goto_state = stoi(value.substr(1));
	return {action, goto_state};
}