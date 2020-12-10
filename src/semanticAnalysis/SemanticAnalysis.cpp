#include "SemanticAnalysis.h"

GrammarSimbol_attributes::GrammarSimbol_attributes(Token token, Token_type tipo, string lexema) {
	this->token = token;
	this->tipo = tipo;
	this->lexema = lexema;
}

GrammarSimbol_attributes::GrammarSimbol_attributes(NonTerminalSimbol token, Token_type tipo, string lexema) {
	this->token = token;
	this->tipo = tipo;
	this->lexema = lexema;
}