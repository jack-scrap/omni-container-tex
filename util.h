#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

namespace util {
	std::string rd(std::string name);

	std::vector<std::string> rdVec(std::string fName);

	namespace str {
		std::vector<std::string> split(std::string buff, char delim);
	}

	namespace mesh {
		namespace rd {
			std::vector<GLfloat> vtc(std::string fName);

			std::vector<GLushort> idc(std::string fName, unsigned int attr);
		}
	}
}
