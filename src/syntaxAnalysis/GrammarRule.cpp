#include "GrammarRule.h"

void GrammarRule::left(NonTerminalSimbol simbol) { grammar_rule.first = simbol; }

void GrammarRule::add_right(Token terminal) {
	GrammarSimbol simbol = {terminal, true};
	grammar_rule.second.push_back(simbol);
}

void GrammarRule::add_right(NonTerminalSimbol nonTerminal) {
	GrammarSimbol simbol;
	simbol.simbol.nonTerminal = nonTerminal;
	simbol.isTerminal = false;
	grammar_rule.second.push_back(simbol);
}

NonTerminalSimbol GrammarRule::left() { return grammar_rule.first; }

vector<GrammarRule::GrammarSimbol> GrammarRule::right() { return grammar_rule.second; }

GrammarRule::operator std::string() {
	std::stringstream ss;
	ss << NonTerminalSimbol_to_string(left()) << " ->";
	for (auto& simbol : grammar_rule.second) ss << ' ' << string(simbol);
	return ss.str();
}