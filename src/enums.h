#ifndef LEXICAL_ENUMS_HEADER
#define LEXICAL_ENUMS_HEADER

#include <string>
#include <stdexcept>

using namespace std;

enum Token {
	inicio = 1,	// inicio palavras reservadas
	varinicio,
	varfim,
	escreva,
	leia,
	se,
	entao,
	fimse,
	fim,
	inteiro,
	lit,
	real,  // fim palavras reservadas
	num,
	literal,
	id,
	EOF_t,
	opr,   // operador relacional
	rcb,   // atribuição
	opm,   // operador aritmetico
	AB_P,  //(
	FC_P,  //)
	PT_V,  //;
	ERRO,
	Comentario,
	Espaco
};

enum Token_type {
	unknow,
	Inteiro,
	Real,
	Cientifico,	 // notação cientifica
	Literal,
	opm_soma,
	opm_sub,
	opm_mult,
	opm_div,
	opr_eq,
	opr_neq,
	opr_less,
	opr_greater,
	opr_less_eq,
	opr_greater_eq,
	atr
};

/*
struct Token_attributes {
	Token token;
	Token_type tipo;
	string lexema;

	inline bool operator==(const Token_attributes& rhs) const { return lexema == rhs.lexema; }
};

template <>
struct std::hash<Token_attributes> {
	inline std::size_t operator()(const Token_attributes& t) const noexcept {
		return std::hash<std::string>{}(t.lexema);
	}
};

struct Token_attr_ptr_hash {
	inline std::size_t operator () (Token_attributes* const &t ) const {
        return std::hash<Token_attributes>{}(*t);
    }
};
struct Token_attr_ptr_eq {
	bool operator () ( Token_attributes const * lhs, Token_attributes const * rhs ) const {
        return lhs->lexema == rhs->lexema;
    }
};
*/

string Token_to_string(Token t);
Token string_to_Token(string str);

string Token_type_to_string(Token_type t);

inline string operator+(string &str, Token_type &orig) { return str + Token_type_to_string(orig); }
inline ostream &operator<<(ostream &os, Token_type &orig) { return os << Token_type_to_string(orig); }



#endif