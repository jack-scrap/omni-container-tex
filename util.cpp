#include <fstream>
#include <iostream>
#include <sstream>

#include "util.h"

template <typename T>
T util::rd(std::string fName) {
}

template <>
std::string util::rd<std::string>(std::string name) {
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

template <>
std::vector<std::string> util::rd<std::vector<std::string>>(std::string fName) {
	std::ifstream in;
	in.open(std::string(std::string(".") + std::string("/") + fName));

	std::vector<std::string> cont;

	for (std::string l; std::getline(in, l);) {
		cont.push_back(l);
	}

	in.close();

	return cont;
}

std::vector<std::string> util::str::split(std::string buff, char delim) {
	std::vector<std::string> tok;

	std::stringstream s(buff);
	std::string seg;
	while (std::getline(s, seg, delim)) {
		tok.push_back(seg);
	}

	return tok;
}

std::vector<GLfloat> util::mesh::rd::vtc(std::string fName) {
	std::vector<GLfloat> _;

	std::vector<std::string> buff = util::rd<std::vector<std::string>>(fName + ".obj");

	for (int l = 0; l < buff.size(); l++) {
		std::vector<std::string> tok = util::str::split(buff[l], ' ');

		if (tok[0] == "v") {
			for (int i = 1; i < 1 + 3; i++) {
				_.push_back(std::stof(tok[i]));
			}
		}
	}

	return _;
}

std::vector<GLushort> util::mesh::rd::idc(std::string fName, unsigned int attr) {
	std::vector<GLushort> _;

	std::vector<std::string> buff = util::rd<std::vector<std::string>>(fName + ".obj");

	for (int l = 0; l < buff.size(); l++) {
		std::vector<std::string> tok = util::str::split(buff[l], ' ');

		if (tok[0] == "f") {
			for (int i = 1; i < 1 + 3; i++) {
				std::vector<std::string> type = util::str::split(tok[i], '/');

				_.push_back(std::stoi(type[attr]) - 1);
			}
		}
	}

	return _;
}
