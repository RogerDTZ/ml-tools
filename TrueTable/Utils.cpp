#include "Utils.h"

#include <iostream>

namespace util {

Operator CharToOperator(char c) {
	switch (c) {
	case '!':
		return Operator::NEG;
	case '&':
		return Operator::AND;
	case '|':
		return Operator::OR;
	case '*':
		return Operator::NAND;
	case ']':
		return Operator::NOR;
	case '-':
		return Operator::IM;
	case '+':
		return Operator::EQ;
	}
	return Operator::NON;
}

std::string OperatorToCmd(Operator opt) {
	switch (opt) {
	case Operator::NEG:
		return R"(\neg)";
	case Operator::AND:
		return R"(\and)";
	case Operator::OR:
		return R"(\or)";
	case Operator::NAND:
		return R"(\uparrow)";
	case Operator::NOR:
		return R"(\downarrow)";
	case Operator::IM:
		return R"(\rightarrow)";
	case Operator::EQ:
		return R"(\leftrightarrow)";
	}
	return "?";
}

std::string preprocessString(std::string str) {
	std::string inStr = str;
	try {
		while (true) {
			unsigned int st = 0, ed = 0;
			bool found = false;
			for (unsigned int i = 0; i < str.length(); ++i)
				if (str[i] == '\\') {
					st = i;
					found = true;
					break;
				}
			if (!found)
				break;
			for (ed = st; ed < str.length() && str[ed] != ' ' && str[ed] != '(' && str[ed] != ')'; ++ed);
			std::string op = str.substr(st, ed - st);
			std::string sub;
			if (op == R"(\neg)")
				sub = "!";
			else if (op == R"(\and)")
				sub = "&";
			else if (op == R"(\or)")
				sub = "|";
			else if (op == R"(\nand)" || op == R"(\na)")
				sub = "*";
			else if (op == R"(\nor)" || op == R"(\no)")
				sub = "]";
			else if (op == R"(\im)")
				sub = "-";
			else if (op == R"(\eq)")
				sub = "+";
			else
				throw std::string("invalid command: " + op);
			str = str.substr(0, st) + sub + str.substr(ed, str.length() - ed);
		}
	} catch (const std::string msg) {
		std::cout << "[ERROR] error when preprocess " << inStr << " : " << msg << std::endl;
		exit(-1);
	}
	return str;
}

}