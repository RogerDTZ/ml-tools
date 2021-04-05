#pragma once

#include <string>
#include <tuple>
#include <memory>

#include "Utils.h"

class Expression {

public:

									Expression();
	void							initAsVar(const int ID, const std::string& name);
	void							initAsConstant(const bool value);
	void							initAsOpr(const Operator& type, std::shared_ptr<Expression> p0, std::shared_ptr<Expression> p1, const int ID);
	void							reset();
	bool							eval();
	Operator						getType();
	void							setVar(bool value);
	int								getID();
	std::tuple<Operator, int, int>	getTuple();
	std::string						getName(bool addBrackets = true);
	bool							isTargetExp();
	void							setTargetExp(bool flag);

private:

	int								ID;
	Operator						type;
	std::string						name;
	std::shared_ptr<Expression>		p0;
	std::shared_ptr<Expression>		p1;
	bool							evaled;
	bool							result;
	bool							isTarget;

};

