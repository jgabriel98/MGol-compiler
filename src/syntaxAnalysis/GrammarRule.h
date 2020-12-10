#ifndef SYNTAX_ANALIZER_GRAMMAR_RULE_HEADER
#define SYNTAX_ANALIZER_GRAMMAR_RULE_HEADER

#include "../enums.h"
#include "enums.h"
#include <vector>

struct GrammarSimbol {
   private:
	union _grammarSimbol_union {
		Token asTerminal;
		NonTerminalSimbol asNonTerminal;
	};

   public:
	union _grammarSimbol_union simbol;
	bool isTerminal;

	inline operator std::string() const {
		return isTerminal ? Token_to_string(simbol.asTerminal) : NonTerminalSimbol_to_string(simbol.asNonTerminal);
	}

	inline GrammarSimbol& operator=(const Token& t) {
		this->simbol.asTerminal = t;
		this->isTerminal = true;
		return *this;
	}
	inline GrammarSimbol operator=(const NonTerminalSimbol& t) {
		this->simbol.asNonTerminal = t;
		this->isTerminal = false;
		return *this;
	}

	inline std::size_t operator()(const GrammarSimbol& t) const noexcept {
		return std::hash<std::size_t>{}(t.simbol.asTerminal);	 // tanto faz .terminal ou .nonTerminal, ambos são enum
	}
	inline bool operator==(const GrammarSimbol& rule) const {
		return (isTerminal == rule.isTerminal) && (simbol.asTerminal == rule.simbol.asTerminal);
	}
	inline bool operator!=(const GrammarSimbol& rule) const { return !(*this == rule); }
};

class GrammarRule {
   private:
	pair<NonTerminalSimbol, vector<GrammarSimbol>> grammar_rule;

   public:
	void left(NonTerminalSimbol simbol);
	void add_right(Token terminal);
	void add_right(NonTerminalSimbol nonTerminal);

	inline NonTerminalSimbol left(){ return grammar_rule.first; }
	inline vector<GrammarSimbol> right(){ return grammar_rule.second; }

	operator std::string();

	inline bool operator==(const GrammarRule& rule) const {
		auto &r1 = grammar_rule, &r2 = rule.grammar_rule;
		if (r1.first != r2.first || r1.second.size() != r2.second.size()) return false;

		auto size = r1.second.size();
		for (int i = 0; i < size; i++)
			if (r1.second[i] != r2.second[i]) return false;

		return true;
	}

	struct hash_func{
	inline std::size_t operator()(const GrammarRule& t) const {
		std::size_t final_hash = std::hash<NonTerminalSimbol>{}(t.grammar_rule.first);
		for (auto& simbol : t.grammar_rule.second) {
			final_hash ^= simbol(simbol);
		}
		return final_hash;
	}
	};
};

#endif