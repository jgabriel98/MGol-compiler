#ifndef GRAMMAR_ATTR_HEADER
#define GRAMMAR_ATTR_HEADER

#include <stack>

#include "../syntaxAnalysis/GrammarRule.h"

using namespace std;

class GrammarSimbol_attributes {
	public: 
	GrammarSimbol token;
	Token_type tipo;
	string lexema;

	GrammarSimbol_attributes() {}

	GrammarSimbol_attributes(Token token, Token_type tipo = unknow, string lexema = "");
	GrammarSimbol_attributes(NonTerminalSimbol token, Token_type tipo = unknow, string lexema = "");

};

template <>
struct std::hash<GrammarSimbol_attributes> {
	inline std::size_t operator()(const GrammarSimbol_attributes& t) const noexcept {
		return std::hash<std::string>{}(t.lexema);
	}
};

struct GrammarSimbol_attr_ptr_hash {
	inline std::size_t operator () (GrammarSimbol_attributes* const &t ) const {
        return std::hash<GrammarSimbol_attributes>{}(*t);
    }
};
struct GrammarSimbol_attr_ptr_eq {
	bool operator () ( GrammarSimbol_attributes const * lhs, GrammarSimbol_attributes const * rhs ) const {
        return lhs->lexema == rhs->lexema;
    }
};

typedef GrammarSimbol_attributes (*semanticRule) (const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int line, const int column);

#endif