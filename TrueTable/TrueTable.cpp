#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Table.h"

void Error(const std::string& msg) {
	std::cout << "[ERROR] " << msg << std::endl;
	exit(-1);
}

int main(int argc, char* argv[]) {
	if (argc < 2)
		Error("too few arguments (at least indicate an input file)");
	std::ifstream fin(argv[1]);
	std::ostream* os;
	if (argc >= 3)
		os = new std::ofstream(argv[2]);
	else
		os = &std::cout;
	if (!fin) {
		Error("cannot open input file" + std::string(argv[1]));
	} else {
		Table table;
		std::string line;
		while (std::getline(fin, line)) {
			if (line == "---") {
				table.flush(*os);
				continue;
			} else if (line == "") {
				continue;
			}
			table.setTargets(line);
		}
		if (!table.empty())
			table.flush(*os);
	}
	return 0;
}