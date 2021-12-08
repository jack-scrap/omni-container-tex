#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

namespace util {
	namespace fs {
		template <typename T>
		T rd(std::string fName);

		template <>
		std::string rd(std::string fName);

		template <>
		std::vector<std::string> rd(std::string fName);

		namespace str {
			std::vector<std::string> split(std::string buff, char delim);
		}
	}

	namespace mesh {
		namespace rd {
			std::vector<GLfloat> vtc(std::string fName);

			std::vector<GLushort> idc(std::string fName, unsigned int attr);
		}
	}
}
