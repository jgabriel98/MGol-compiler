#include "MySemanticRulesList.h"
#include "../color.h"

#include <iostream>


inline string join_lexema(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr){
	string out;
	for(auto &s: right_rule_simbols_attr)
		out+= s->lexema;
	return out;
}

inline string typeToC(Token_type type){
	    switch(type){
        case unknow: return "?";
        case Inteiro: return "int";
        case Real: return "double";
        case Cientifico: return "Cientifico";
		case Literal: return "literal";
		case opm_soma: return "+";
		case opm_sub: return "-";
		case opm_mult: return "*";
		case opm_div: return "/";
		case opr_eq: return "=";
		case opr_neq: return "!=";
		case opr_less: return "<";
		case opr_greater: return ">";
		case opr_less_eq: return "<=";
		case opr_greater_eq: return ">=";
		case atr: return "=";
    }
	return nullptr;
}




FILE* MySemanticRulesList::C_code_file = nullptr;
char MySemanticRulesList::buff[512];
stringstream MySemanticRulesList::C_code_stream;

SimbolsTable* MySemanticRulesList::simbolsTable = nullptr;
unsigned int MySemanticRulesList::T_counter = 0;
stringstream MySemanticRulesList::Tx_stream;
unsigned char MySemanticRulesList::identation = 1;


void MySemanticRulesList::configure(FILE* C_code_output_file, SimbolsTable *simbolsTable){
	C_code_file = C_code_output_file;
	MySemanticRulesList::simbolsTable = simbolsTable;
	C_code_stream.clear();
}

void MySemanticRulesList::create_Tx(GrammarSimbol_attributes *simbol, Token_type type) {
	simbol->tipo = type;
	simbol->lexema = string("T") + to_string(T_counter++);
	sprintf(buff, "%s %s;\n", typeToC(simbol->tipo).c_str(), simbol->lexema.c_str());
	Tx_stream << string(1, '\t') << buff;
}


GrammarSimbol_attributes MySemanticRulesList::rule_2(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	stringstream temp;
	temp <<"#include<stdio.h>\ntypedef char literal[256];\nint main(void){\n\n";
	temp << C_code_stream.rdbuf();
	C_code_stream.swap(temp); // = std::move(temp);
	return {NonTerminalSimbol::P};
}

GrammarSimbol_attributes MySemanticRulesList::rule_5(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	C_code_stream << string(identation, '\t') <<TX_Vars_Delimiter;
	C_code_stream << string(identation, '\t') << TX_Vars_Delimiter_End;
	C_code_stream << string(identation, '\t') <<"\n\n\n";
	return GrammarSimbol_attributes(NonTerminalSimbol::LV, unknow, join_lexema(right_rule_simbols_attr));
}

GrammarSimbol_attributes MySemanticRulesList::rule_6(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	auto id = right_rule_simbols_attr[0], TIPO = right_rule_simbols_attr[1], PT_V = right_rule_simbols_attr[2];

	id->tipo = TIPO->tipo;
	sprintf(buff, "%s %s;\n", typeToC(TIPO->tipo).c_str(), id->lexema.c_str());
	C_code_stream <<string(identation, '\t')<< buff;
	
	return {NonTerminalSimbol::D, unknow, join_lexema(right_rule_simbols_attr)};
}

GrammarSimbol_attributes MySemanticRulesList::var_declaration_rule (const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	GrammarSimbol_attributes TIPO{NonTerminalSimbol::TIPO, right_rule_simbols_attr[0]->tipo, right_rule_simbols_attr[0]->lexema};
	return TIPO;
}

GrammarSimbol_attributes MySemanticRulesList::rule_11(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	auto leia = right_rule_simbols_attr[0], id = right_rule_simbols_attr[1], PT_V = right_rule_simbols_attr[2];
	if(id->tipo != Token_type::unknow){
		switch(id->tipo){
		case Token_type::Literal: 
			sprintf(buff, "scanf(\"%%s\", %s);\n", id->lexema.c_str());
			break;
		case Token_type::Inteiro:
			sprintf(buff, "scanf(\"%%d\", &%s);\n", id->lexema.c_str());
			break;
		case Token_type::Real:
			sprintf(buff, "scanf(\"%%lf\", &%s);\n", id->lexema.c_str());
			break;
		}
		C_code_stream << string(identation, '\t') << buff;
	}
	else{
		cout << RED("Erro semântico: variável ") << SetForeYEL << id->lexema << RED(" não declarada.") << " Linha " << l
			 << " e coluna " << c << endl;
	}

	return {NonTerminalSimbol::ES, unknow, join_lexema(right_rule_simbols_attr)};
}


GrammarSimbol_attributes MySemanticRulesList::rule_12(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	auto escreva = right_rule_simbols_attr[0], ARG = right_rule_simbols_attr[1], PT_V = right_rule_simbols_attr[2];
	
	char printf_flag = '?';
	switch(ARG->tipo){
		case Token_type::Literal: printf_flag = 's'; break;
		case Token_type::Inteiro: printf_flag = 'd'; break;
		case Token_type::Real: 	  printf_flag = 'f'; break;
	}
	
	sprintf(buff, "printf(\"%%%c\", %s);\n", printf_flag, ARG->lexema.c_str());
	C_code_stream << string(identation, '\t') << buff;
	return {NonTerminalSimbol::ES, unknow, join_lexema(right_rule_simbols_attr)};
}

GrammarSimbol_attributes MySemanticRulesList::rule_13(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	auto literal = right_rule_simbols_attr[0];
	GrammarSimbol_attributes ARG = {NonTerminalSimbol::ARG, literal->tipo, literal->lexema};
	return ARG;
}

GrammarSimbol_attributes MySemanticRulesList::rule_14(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	auto num = right_rule_simbols_attr[0];
	GrammarSimbol_attributes ARG = {NonTerminalSimbol::ARG, num->tipo, num->lexema};
	return ARG;
}

GrammarSimbol_attributes MySemanticRulesList::rule_15(const vector<GrammarSimbol_attributes*> right_rule_simbols_attr, const int l, const int c) {
	auto id = right_rule_simbols_attr[0];
	GrammarSimbol_attributes ARG = {NonTerminalSimbol::ARG, unknow, id->lexema};
	if (id->tipo != Token_type::unknow) {
		ARG.tipo = id->tipo;
	} else {
		cout << RED("Erro semântico: variável ") << SetForeYEL << id->lexema << RED(" não declarada.") << " Linha " << l
			 << " e coluna " << c << endl;
	}

	return ARG;
}


GrammarSimbol_attributes MySemanticRulesList::rule_17(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c) {
	auto id = right_rule_simbols_attr[0], rcb = right_rule_simbols_attr[1], LD = right_rule_simbols_attr[2];

	if (id->tipo != Token_type::unknow) {

		if (LD->tipo == id->tipo) {
			sprintf(buff, "%s %s %s;\n", id->lexema.c_str(), typeToC(rcb->tipo).c_str(), LD->lexema.c_str());
			C_code_stream << string(identation, '\t') << buff;
		} else {
			cout << RED("Erro semântico: Tipos diferentes para atribuição: ") << SetForeYEL << id->lexema << WHT(" e ")
				 << SetForeYEL << LD->lexema << WHT(" (") << SetForeYEL << Token_type_to_string(id->tipo) << WHT(" e ")
				 << SetForeYEL << Token_type_to_string(LD->tipo) << WHT(")") << ". Linha " << l << " e coluna " << c
				 << endl;
		}
		
	} else {
		cout << RED("Erro semântico: variável ") << SetForeYEL << id->lexema << RED(" não declarada.") << " Linha " << l
			 << " e coluna " << c << endl;
	}

	return {NonTerminalSimbol::CMD, unknow, join_lexema(right_rule_simbols_attr)};
}

GrammarSimbol_attributes MySemanticRulesList::rule_18(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	auto OPRD_1 = right_rule_simbols_attr[0], opm = right_rule_simbols_attr[1], OPRD_2 = right_rule_simbols_attr[2];
	GrammarSimbol_attributes LD = {NonTerminalSimbol::LD};

	if (OPRD_1->tipo == OPRD_2->tipo && OPRD_1->tipo != Token_type::Literal) {
		
		create_Tx(&LD, OPRD_1->tipo);
		sprintf(buff, "%s = %s %s %s;\n", LD.lexema.c_str(), OPRD_1->lexema.c_str(), typeToC(opm->tipo).c_str(), OPRD_2->lexema.c_str());
		C_code_stream << string(identation, '\t') << buff;
		
	} else {
		cout << RED("Erro semântico: Operandos com tipos incompatíveis: ") << SetForeYEL << Token_type_to_string(OPRD_1->tipo)
			 << WHT(" e ") << Token_type_to_string(OPRD_2->tipo) << ". Linha " << l << " coluna" << c <<
			endl;
	}

	return LD;
}

GrammarSimbol_attributes MySemanticRulesList::rule_19(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	GrammarSimbol_attributes *OPRD = right_rule_simbols_attr[0], LD = {NonTerminalSimbol::LD, OPRD->tipo, OPRD->lexema};
	return LD;
}

GrammarSimbol_attributes MySemanticRulesList::rule_20(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	GrammarSimbol_attributes *id = right_rule_simbols_attr[0], OPRD = {NonTerminalSimbol::OPRD, unknow, id->lexema};
	if (id->tipo != Token_type::unknow) {
		OPRD.tipo = id->tipo;
	} else {
		cout << RED("Erro semântico: variável ") << SetForeYEL << id->lexema << RED(" não declarada.") << " Linha " << l
			 << " e coluna " << c << endl;
	}
	return OPRD;
}

GrammarSimbol_attributes MySemanticRulesList::rule_21(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	GrammarSimbol_attributes *num = right_rule_simbols_attr[0], OPRD = {NonTerminalSimbol::OPRD, num->tipo, num->lexema};
	return OPRD;
}

GrammarSimbol_attributes MySemanticRulesList::rule_23(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	identation--;
	C_code_stream << string(identation, '\t') << "}\n";
	GrammarSimbol_attributes COND = {NonTerminalSimbol::COND, unknow, join_lexema(right_rule_simbols_attr)};
	return COND;
}

GrammarSimbol_attributes MySemanticRulesList::rule_24(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	auto EXP_R = right_rule_simbols_attr[2];
	sprintf(buff, "if(%s){\n", EXP_R->lexema.c_str());
	C_code_stream << string(identation, '\t') << buff;
	identation++;

	GrammarSimbol_attributes CABECALHO = {NonTerminalSimbol::CABECALHO, unknow, join_lexema(right_rule_simbols_attr)};
	return CABECALHO;
}

GrammarSimbol_attributes MySemanticRulesList::rule_25(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	auto OPRD_1 = right_rule_simbols_attr[0], opr = right_rule_simbols_attr[1], OPRD_2 = right_rule_simbols_attr[2];
	GrammarSimbol_attributes EXP_R = {NonTerminalSimbol::EXP_R};

	if ((OPRD_1->tipo == OPRD_2->tipo) || (
		(OPRD_1->tipo == Inteiro || OPRD_1->tipo == Real || OPRD_1->tipo == Cientifico) &&
		(OPRD_2->tipo == Inteiro || OPRD_2->tipo == Real || OPRD_2->tipo == Cientifico) )) {
		
		create_Tx(&EXP_R, Inteiro);

		sprintf(buff, "%s=%s%s%s;\n", EXP_R.lexema.c_str(), OPRD_1->lexema.c_str(), typeToC(opr->tipo).c_str(), OPRD_2->lexema.c_str());
		C_code_stream << string(identation, '\t') << buff;

	}else{
		cout << RED("Erro semântico: Operandos com tipos incompatíveis: ") << SetForeYEL << Token_type_to_string(OPRD_1->tipo)
			 << WHT(" e ") << Token_type_to_string(OPRD_2->tipo) << ". Linha " << l << " coluna" << c <<
			endl;
	}
	return EXP_R;
}

GrammarSimbol_attributes MySemanticRulesList::rule_30(const vector<GrammarSimbol_attributes *> right_rule_simbols_attr, const int l, const int c){
	identation--;
	C_code_stream << string(identation, '\t') << "}\n";
	return {NonTerminalSimbol::A};
}
