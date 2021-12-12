#pragma once

#define VTX_SZ 3
#define ST_SZ 2

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "prog.h"

class Prop {
	private:
		unsigned int _noEl;

		GLuint _vao;

		GLuint _vbo;
		GLuint _stbo;

		GLuint _ibo;

		GLuint _tex;

		Prog _prog;
		Prog _progOutline;
		Prog _progWire;

		GLint _attrPos;
		GLint _attrSt;

		GLint _uniModel;
		GLint _uniView;
		GLint _uniProj;

		GLint _uniModelOutline;
		GLint _uniViewOutline;
		GLint _uniProjOutline;

		GLint _uniModelWire;
		GLint _uniViewWire;
		GLint _uniProjWire;

		GLuint _uniTex;

	public:
		glm::mat4 _model;
		glm::mat4 _view;
		glm::mat4 _proj;

		bool _showWire = false;

		Prop(std::string modelName, std::string texName);

		void draw();
};
