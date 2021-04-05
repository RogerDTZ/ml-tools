#include "Table.h"

#include <stack>

Table::Table() {
	this->reset();
}

void Table::setTargets(std::string expression) {
	try {
		this->initNodes(expression);
	} catch (const char* msg) {
		std::cout << "[ERROR]" << msg << std::endl;
		exit(-1);
	}
}

bool Table::empty() {
	return this->nodes.empty();
}

void Table::reset() {
	this->idMap.clear();
	this->nameMap.clear();
	this->nodes.clear();
	this->vars.clear();
	idCnt = 30;
	nameCnt = 0;
}

void Table::initNodes(std::string& str) {
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
					throw "Error format: bracket cannot match!";
				} else {
					unsigned int lpos = lb.top();
					lb.pop();
					if (ele.empty() || ele.top().first < lpos) {
						throw "Error format: empty pair of brackets!";
					} else {
						std::pair<unsigned int, std::shared_ptr<Expression>> lhs, rhs;
						rhs = ele.top();
						ele.pop();
						if (ele.empty() || ele.top().first < lpos) {
							if (opr.size() > 0 && opr.top() > lpos) {
								Operator op = util::CharToOperator(str[opr.top()]);
								if (!(op == Operator::NEG && opr.top() < rhs.first)) {
									throw "Error format: illegal operation on single expression!";
								}
								opr.pop();
								std::shared_ptr<Expression> exp(new Expression());
								exp->initAsOpr(Operator::NEG, rhs.second, nullptr, idCnt);
								std::tuple<Operator, int, int> tuple = exp->getTuple();
								if (this->idMap.find(tuple) == this->idMap.end()) {
									idMap[tuple] = exp;
									this->nodes.push_back(exp);
									++idCnt;
								} else {
									exp = idMap[tuple];
								}
								ele.push(std::make_pair(lpos, exp));
							} else {
								ele.push(rhs);
							}
							continue;
						}
						lhs = ele.top();
						ele.pop();
						if (ele.size() > 0 && ele.top().first > lpos) {
							throw "Error format: too many subexpressions in one pair of brackets!";
						}
						if (opr.empty() || opr.top() < lpos) {
							throw "Error format: NO operator within a expression!";
						}
						if (!(lhs.first < opr.top() && opr.top() < rhs.first)) {
							throw "Error format: please check the order!";
						}
						Operator op = util::CharToOperator(str[opr.top()]);
						if (op == Operator::NON) {
							throw ("Error symbol: unrecognized operator " + std::string(1, str[opr.top()])).c_str();
						}
						opr.pop();
						std::shared_ptr<Expression> exp(new Expression());
						exp->initAsOpr(op, lhs.second, rhs.second, idCnt);
						std::tuple<Operator, int, int> tuple = exp->getTuple();
						if (idMap.find(tuple) == idMap.end()) {
							idMap[tuple] = exp;
							this->nodes.push_back(exp);
							++idCnt;
						} else {
							exp = idMap[tuple];
						}
						ele.push(std::make_pair(lpos, exp));
					}
				}
			} else if (str[i] != ' ') {
				opr.push(i);
			}
		}
	}
	if (!opr.empty())
		throw "Extra operators!";
	if (!lb.empty())
		throw "Extra left braket!";
	if (ele.size() > 1)
		throw "Extra expression!";
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
	os << std::endl << std::endl << std::endl << std::endl;
	this->reset();
}
