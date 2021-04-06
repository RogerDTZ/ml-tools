#pragma once

#include <string>

enum class Operator { 
	NON,
	VAR,
	T,
	F,
	NEG,
	AND,
	OR,
	NAND,
	NOR,
	IM,
	EQ
};

namespace util {


Operator	CharToOperator(char c);

std::string OperatorToCmd(Operator opt);

std::string PreprocessString(std::string str);

}
