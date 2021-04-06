#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#include "Utils.h"
#include "Expression.h"

const unsigned int VarMaxNum = 50;

class Table {

public:

	Table();

	void flush(std::ostream& os);
	void setTargets(std::string expression);
	bool empty();


private:

	std::map<std::tuple<Operator, int, int>, std::shared_ptr<Expression>>	idMap;
	std::map<std::string, int>												nameMap;
	std::vector<std::shared_ptr<Expression>>								nodes;
	std::vector<std::shared_ptr<Expression>>								vars;
	int																		idCnt;
	int																		nameCnt;
	int																		targetCnt;

	void initNodes(std::string str);
	void printTableHeaders(std::ostream& os);
	void printTableRow(std::ostream& os);
	void reset();

};
