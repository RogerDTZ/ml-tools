#include "Table.h"

#include <stack>

Table::Table() {
	this->reset();
}

void Table::setTargets(std::string expression) {
	try {
		this->initNodes(expression);
	} catch (const std::string msg) {
		std::cout << "[ERROR] error in expression " << expression << " : " << msg << std::endl;
		exit(-1);
	}
	std::cout << "[TTable] Successfully added expression " << expression << std::endl;
	++this->targetCnt;
}

bool Table::empty() {
	return this->targetCnt == 0;
}

void Table::reset() {
	this->idMap.clear();
	this->nameMap.clear();
	this->nodes.clear();
	this->vars.clear();
	this->idCnt = VarMaxNum;
	this->nameCnt = 0;
	this->targetCnt = 0;
}

void Table::initNodes(std::string str) {
	str = "(" + str + ")";
	std::stack<unsigned> lb;
	std::stack<unsigned> opr;
	std::stack<std::pair<unsigned int, std::shared_ptr<Expression>>> ele;
	std::string curName = "";
	for (unsigned int i = 0; i < str.length(); ++i) {
		if ('A' <= str[i] && str[i] <= 'Z' || 'a' <= str[i] && str[i] <= 'z' || '0' <= str[i] && str[i] <= '9' || str[i] == '_') {
			curName = curName + std::string(1, str[i]);
		} else {
			if (curName.length() > 0) {
				std::shared_ptr<Expression> exp(new Expression());
				if (curName == "T") {
					exp->initAsConstant(true);
				} else if (curName == "F") {
					exp->initAsConstant(false);
				} else {
					if (this->nameMap.find(curName) == this->nameMap.end()) {
						this->nameMap[curName] = nameCnt++;
					}
					exp->initAsVar(this->nameMap[curName], curName);
					std::tuple<Operator, int, int> tuple = exp->getTuple();
					if (this->idMap.find(tuple) == this->idMap.end()) {
						idMap[tuple] = exp;
						this->nodes.push_back(exp);
						this->vars.push_back(exp);
						std::cout << "[Symbol] new variable(" << this->nodes.size() << "): " << curName << std::endl;
						if (this->nodes.size() > 25) {
							std::cout << "[Warning] quite a few variables." << std::endl;
						}
						if (this->nodes.size() > VarMaxNum) {
							throw std::string("computational error: too many variables! (limit=" + std::to_string(VarMaxNum)) + ")";
						}
					} else {
						exp = idMap[tuple];
					}
				}
				ele.push(std::make_pair(i, exp));
				curName = "";
			}
			if (str[i] == '(') {
				lb.push(i);
			} else if (str[i] == ')') {
				if (lb.empty()) {
					throw std::string("error format: bracket cannot match!");
				}
				unsigned int lpos = lb.top();
				lb.pop();
				if (ele.empty() || ele.top().first < lpos) {
					throw std::string("error format: empty pair of brackets!");
				}
				std::pair<unsigned int, std::shared_ptr<Expression>> lhs, rhs;
				rhs = ele.top();
				ele.pop();
				if (ele.empty() || ele.top().first < lpos) { // single exp
					if (opr.size() > 0 && opr.top() > lpos) {
						Operator op = util::CharToOperator(str[opr.top()]);
						if (!(op == Operator::NEG && opr.top() < rhs.first)) {
							throw std::string("error format: invalid operator!");
						}
						opr.pop();
						std::shared_ptr<Expression> exp(new Expression());
						exp->initAsOpr(Operator::NEG, rhs.second, nullptr, idCnt);
						std::tuple<Operator, int, int> tuple = exp->getTuple();
						if (this->idMap.find(tuple) == this->idMap.end()) {
							this->idMap[tuple] = exp;
							this->nodes.push_back(exp);
							++idCnt;
							std::cout << "[Symbol] new expression: " << exp->getName() << std::endl;
						} else {
							exp = idMap[tuple];
						}
						ele.push(std::make_pair(lpos, exp));
					} else {
						ele.push(rhs);
					}
				} else { // paired exp
					lhs = ele.top();
					ele.pop();
					if (ele.size() > 0 && ele.top().first > lpos) {
						throw std::string("error format: too many subexpressions in one pair of brackets!");
					}
					if (opr.empty() || opr.top() < lpos) {
						throw std::string("error format: NO operator in a pair of brackets!");
					}
					if (!(lhs.first < opr.top() && opr.top() < rhs.first)) {
						throw std::string("error format: please check the order between expressions and operators!");
					}
					Operator op = util::CharToOperator(str[opr.top()]);
					if (op == Operator::NON) {
						throw std::string("error symbol: unrecognized operator " + std::string(1, str[opr.top()]));
					}
					opr.pop();
					std::shared_ptr<Expression> exp(new Expression());
					exp->initAsOpr(op, lhs.second, rhs.second, idCnt);
					std::tuple<Operator, int, int> tuple = exp->getTuple();
					if (this->idMap.find(tuple) == this->idMap.end()) {
						this->idMap[tuple] = exp;
						this->nodes.push_back(exp);
						++idCnt;
						std::cout << "[Symbol] New expression: " << exp->getName() << std::endl;
					} else {
						exp = idMap[tuple];
					}
					ele.push(std::make_pair(lpos, exp));
				}
			} else if (str[i] != ' ') {
				opr.push(i);
			} 
		}
	}
	if (!opr.empty())
		throw std::string("error format: extra operators at the end!");
	if (!lb.empty())
		throw std::string("error format: left brackets not matched!");
	if (ele.size() > 1)
		throw std::string("error format: expression illegal! (not a single one)");
	this->nodes[this->nodes.size() - 1]->setTargetExp(true);
}

void Table::printTableHeaders(std::ostream& os) {
	os << "|";
	for (auto exp : this->nodes)
		if (exp->getType() == Operator::VAR) {
			os << " " << "$" << exp->getName(false) << "$" << " |";
		}
	for (auto exp : this->nodes)
		if (exp->getType() != Operator::VAR && exp->getType() != Operator::T && exp->getType() != Operator::F && !exp->isTargetExp()) {
			os << " " << "$" << exp->getName(false) << "$" << " |";
		}
	for (auto exp : this->nodes)
		if (exp->getType() != Operator::VAR && exp->getType() != Operator::T && exp->getType() != Operator::F && exp->isTargetExp()) {
			os << " " << "$" << exp->getName(false) << "$" << " |";
		}
	os << std::endl;
	os << "|";
	for (auto exp : this->nodes)
		if (exp->getType() == Operator::VAR) {
			os << " " << ":-:" << " |";
		}
	for (auto exp : this->nodes)
		if (exp->getType() != Operator::VAR && exp->getType() != Operator::T && exp->getType() != Operator::F && !exp->isTargetExp()) {
			os << " " << ":-:" << " |";
		}
	for (auto exp : this->nodes)
		if (exp->getType() != Operator::VAR && exp->getType() != Operator::T && exp->getType() != Operator::F && exp->isTargetExp()) {
			os << " " << ":-:" << " |";
		}
	os << std::endl;
}

void Table::printTableRow(std::ostream& os) {
	os << "|";
	for (auto exp : this->nodes)
		if (exp->getType() == Operator::VAR) {
			os << " " << (exp->eval() ? "T" : "F") << " |";
		}
	for (auto exp : this->nodes)
		if (exp->getType() != Operator::VAR && exp->getType() != Operator::T && exp->getType() != Operator::F && !exp->isTargetExp()) {
			os << " " << (exp->eval() ? "T" : "F") << " |";
		}
	for (auto exp : this->nodes)
		if (exp->getType() != Operator::VAR && exp->getType() != Operator::T && exp->getType() != Operator::F && exp->isTargetExp()) {
			os << " " << (exp->eval() ? "T" : "F") << " |";
		}
	os << std::endl;
}

void Table::flush(std::ostream& os) {
	std::cout << "[TTable] printinig result..." << std::endl;
	std::cout << " - target expression: " << this->targetCnt << std::endl;
	std::cout << " - total  expression: " << this->nodes.size() << std::endl;
	std::cout << " - total  variable  : " << this->vars.size() << std::endl;
	std::cout << " - rows   count     : " << (1 << this->vars.size()) << std::endl;
	std::cout << " - total  expense   : " << 1ll * (1ll << this->vars.size()) * this->nodes.size() << std::endl;
	this->printTableHeaders(os);
	unsigned int varCnt = this->vars.size();
	unsigned int state = 1 << varCnt;
	for (int s = state - 1; s >= 0; --s) {
		for (auto exp : this->nodes)
			exp->reset();
		for (unsigned int i = 0; i < varCnt; ++i) {
			this->vars[i]->setVar((s >> (varCnt - 1 - i)) & 1);
		}
		for (auto exp : this->nodes)
			exp->eval();
		this->printTableRow(os);
	}
	std::cout << "[TTable] table generated successfully" << std::endl;
	std::cout << "[TTable] buffer cleared. " << std::endl << std::endl;
	os << std::endl << std::endl << std::endl << std::endl;
	this->reset();
}
