#pragma once

#define VTX_SZ 3
#define ST_SZ 2

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "prog.h"

class Obj {
	private:
		unsigned int _noEl;

		GLuint _vao;
		GLuint _vbo;

		GLuint _ibo;

		Prog _prog;
		Prog _progOutline;

		GLint _uniModel;
		GLint _uniView;
		GLint _uniProj;

		GLint _uniModelOutline;
		GLint _uniViewOutline;
		GLint _uniProjOutline;

	public:
		glm::mat4 _model;
		glm::mat4 _view;
		glm::mat4 _proj;

		Obj(std::string name, std::string texName);

		void draw();
};
