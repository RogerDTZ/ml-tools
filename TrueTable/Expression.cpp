#include "Expression.h"

Expression::Expression() 
	: ID(-1), type(Operator::NON), p0(nullptr), p1(nullptr), evaled(false), result(false), isTarget(false) {
}

void Expression::initAsVar(const int ID, const std::string& name) {
	this->type = Operator::VAR;
	this->name = name;
	this->ID = ID;
}

void Expression::initAsConstant(const bool value) {
	this->type = value ? Operator::T : Operator::F;
	this->evaled = true;
	this->result = value;
	this->ID = value ? 100 : 101;
	this->name = value ? "T" : "F";
}

void Expression::initAsOpr(const Operator& type, std::shared_ptr<Expression> p0, std::shared_ptr<Expression> p1, const int ID) {
	this->type = type;
	if ((this->type == Operator::IM || this->type == Operator::EQ) || p1 == nullptr || p0->getID() < p1->getID()) {
		this->p0 = p0;
		this->p1 = p1;
	} else {
		this->p0 = p1;
		this->p1 = p0;
	}
	if (this->type == Operator::NEG) {
		this->name = R"(\neg)" + std::string(" ") + p0->getName();
	} else {
		this->name = p0->getName() + std::string(" ") + util::OperatorToCmd(type) + std::string(" ") + p1->getName();
	}
	this->ID = ID;
}

Operator Expression::getType() {
	return this->type;
}

void Expression::reset() {
	if (this->type != Operator::T && this->type != Operator::F) {
		this->evaled = false;
		this->result = false;
	}
}

bool Expression::eval() {
	if (this->evaled)
		return this->result;
	this->evaled = true;
	if (this->type == Operator::T) {
		this->result = true;
	} else if (this->type == Operator::F) {
		this->result = false;
	} else if (this->type == Operator::NEG) {
		this->result = !this->p0->eval();
	} else if (this->type == Operator::AND) {
		this->result = this->p0->eval() & this->p1->eval();
	} else if (this->type == Operator::OR) {
		this->result = this->p0->eval() | this->p1->eval();
	} else if (this->type == Operator::NAND) {
		this->result = !(this->p0->eval() & this->p1->eval());
	} else if (this->type == Operator::NOR) {
		this->result = !(this->p0->eval() | this->p1->eval());
	} else if (this->type == Operator::IM) {
		this->result = !(this->p0->eval() & !this->p1->eval());
	} else if (this->type == Operator::EQ) {
		this->result = this->p0->eval() == this->p1->eval();
	}
	return this->result;
}

void Expression::setVar(bool value) {
	if (this->type != Operator::VAR)
		return;
	this->evaled = true;
	this->result = value;
}

int Expression::getID() {
	return this->ID;
}

std::tuple<Operator, int, int> Expression::getTuple() {
	if (this->type == Operator::T || this->type == Operator::F)
		return std::tuple<Operator, int, int>(this->type, -1, -1);
	if (this->type == Operator::VAR)
		return std::tuple<Operator, int, int>(this->type, ID, -1);
	if (this->type == Operator::NEG)
		return std::tuple<Operator, int, int>(this->type, p0->getID(), -1);
	return std::tuple<Operator, int, int>(this->type, p0->getID(), p1->getID());
}

std::string Expression::getName(bool addBrackets) {
	return this->name.length() > 1 && addBrackets ? std::string("(") + this->name + std::string(")") : this->name;
}

bool Expression::isTargetExp() {
	return this->isTarget;
}

void Expression::setTargetExp(bool flag) {
	this->isTarget = flag;
}
