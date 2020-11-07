#include "ActionTable.h"
#include "CSVReader.h"

#include "../helper.h"

ActionTable::ActionTable(string table_csv) {
	this->table = CSVReader(table_csv).getData();
	auto& header = table[0];
	for (int c = 0; c < header.size(); c++) {
		Token t = string_to_Token(header[c]);
		terminal_to_columIdx[t] = c;
	}
}
// todo: ao inves de converter toda vez, é melhor pre-processar tudo no contrutor.
pair<Action, int> ActionTable::get(int state, Token terminal_simbol) {
	int c = terminal_to_columIdx[terminal_simbol];
	string& value = table[state][c];

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