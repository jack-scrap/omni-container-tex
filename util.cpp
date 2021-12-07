#include <fstream>
#include <iostream>

#include "util.h"

std::string util::rd(std::string name) {
	std::ifstream in;
	in.open("./" + name);

	std::string total;
	for (
		std::string l;
		std::getline(in, l);
	) {
		total += l + "\n";
	}

	in.close();

	return total;
}

std::vector<std::string> util::rdVec(std::string fName) {
	std::ifstream in;
	in.open(std::string(std::string(".") + std::string("/") + fName));

	std::vector<std::string> cont;

	for (std::string l; std::getline(in, l);) {
		cont.push_back(l);
	}

	in.close();

	return cont;
}
