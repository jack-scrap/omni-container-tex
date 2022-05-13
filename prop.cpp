#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "prop.h"
#include "math.h"
#include "stb_image.h"

Prop::Prop(std::string modelName, std::string texName) :
	_prog("res/shad/tex", "res/shad/tex"),
	_progBackdrop("res/shad/backdrop", "res/shad/backdrop"),
	_progWire("res/shad/base", "res/shad/solid") {
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		std::vector<GLfloat> vtcRaw = util::mesh::rd::vtc("res/obj/" + modelName);
		std::vector<GLushort> vtcIdc = util::mesh::rd::idc("res/obj/" + modelName, 0);

		_noEl = vtcIdc.size();

		std::vector<GLfloat> vtc;
		for (int i = 0; i < vtcIdc.size(); i++) {
			for (int a = 0; a < VTX_SZ; a++) {
				vtc.push_back(vtcRaw[(vtcIdc[i] * VTX_SZ) + a]);
			}
		}
		glBufferData(GL_ARRAY_BUFFER, vtc.size() * sizeof (GLfloat), &vtc[0], GL_STATIC_DRAW);

		glGenBuffers(1, &_stbo);
		glBindBuffer(GL_ARRAY_BUFFER, _stbo);

		std::vector<GLfloat> stRaw = util::mesh::rd::st("res/obj/" + modelName);
		std::vector<GLushort> stIdc = util::mesh::rd::idc("res/obj/" + modelName, 1);

		std::vector<GLfloat> st;
		for (int i = 0; i < stIdc.size(); i++) {
			for (int a = 0; a < ST_SZ; a++) {
				st.push_back(stRaw[(stIdc[i] * ST_SZ) + a]);
			}
		}
		glBufferData(GL_ARRAY_BUFFER, st.size() * sizeof (GLfloat), &st[0], GL_STATIC_DRAW);

		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		std::vector<GLushort> idc;
		for (int i = 0; i < vtc.size() / 3; i++) {
			idc.push_back(i);
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idc.size() * sizeof (GLushort), &idc[0], GL_STATIC_DRAW);

		_model = glm::mat4(1.0);
		_view = glm::lookAt(glm::vec3(3.0, 3.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
		_proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

		_prog.use();

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		_attrPos = glGetAttribLocation(_prog._id, "pos");
		glVertexAttribPointer(_attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(_attrPos);

		glBindBuffer(GL_ARRAY_BUFFER, _stbo);
		_attrSt = glGetAttribLocation(_prog._id, "st");
		glVertexAttribPointer(_attrSt, ST_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(_attrSt);

		_uniModel = glGetUniformLocation(_prog._id, "model");
		_uniView = glGetUniformLocation(_prog._id, "view");
		_uniProj = glGetUniformLocation(_prog._id, "proj");

		_uniTex = glGetUniformLocation(_prog._id, "tex");

		_tex;
		glGenTextures(1, &_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int sz[2];
		int chan;
		GLubyte* data = stbi_load(std::string("res/tex/" + texName + ".bmp").c_str(), &sz[X], &sz[Y], &chan, 3);
		if (!data) {
			std::cout << "Error: Couldn't load" << std::endl;

			std::cout << stbi_failure_reason() << std::endl;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sz[X], sz[Y], 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glUniformMatrix4fv(_uniProj, 1, GL_FALSE, glm::value_ptr(_proj));
		glUniformMatrix4fv(_uniView, 1, GL_FALSE, glm::value_ptr(_view));
		glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(_model));

		_prog.unUse();

		_progBackdrop.use();

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		GLint attrPosBackdrop = glGetAttribLocation(_prog._id, "pos");
		glVertexAttribPointer(attrPosBackdrop, VTX_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(attrPosBackdrop);

		_uniModelBackdrop = glGetUniformLocation(_progBackdrop._id, "model");
		_uniViewBackdrop = glGetUniformLocation(_progBackdrop._id, "view");
		_uniProjBackdrop = glGetUniformLocation(_progBackdrop._id, "proj");

		glUniformMatrix4fv(_uniProjBackdrop, 1, GL_FALSE, glm::value_ptr(_proj));
		glUniformMatrix4fv(_uniViewBackdrop, 1, GL_FALSE, glm::value_ptr(_view));
		glUniformMatrix4fv(_uniModelBackdrop, 1, GL_FALSE, glm::value_ptr(_model));

		_progBackdrop.unUse();

		_progWire.use();

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		GLint attrPosWire = glGetAttribLocation(_prog._id, "pos");
		glVertexAttribPointer(attrPosWire, VTX_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(attrPosWire);

		_uniModelWire = glGetUniformLocation(_progWire._id, "model");
		_uniViewWire = glGetUniformLocation(_progWire._id, "view");
		_uniProjWire = glGetUniformLocation(_progWire._id, "proj");

		glUniformMatrix4fv(_uniProjWire, 1, GL_FALSE, glm::value_ptr(_proj));
		glUniformMatrix4fv(_uniViewWire, 1, GL_FALSE, glm::value_ptr(_view));
		glUniformMatrix4fv(_uniModelWire, 1, GL_FALSE, glm::value_ptr(_model));

		_progWire.unUse();
	}

void Prop::draw() {
	glDisable(GL_DEPTH_TEST);

	_progBackdrop.use();

	glUniformMatrix4fv(_uniModelBackdrop, 1, GL_FALSE, glm::value_ptr(_model));

	glDrawArrays(GL_TRIANGLES, 0, _noEl);

	_progBackdrop.unUse();

	glEnable(GL_DEPTH_TEST);

	_prog.use();

	glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(_model));

	glDrawElements(GL_TRIANGLES, _noEl, GL_UNSIGNED_SHORT, (GLvoid*) 0);

	_prog.unUse();

	if (_showWire) {
		_progWire.use();

		glUniformMatrix4fv(_uniModelWire, 1, GL_FALSE, glm::value_ptr(_model));

		glDrawArrays(GL_LINES, 0, _noEl);

		_progWire.unUse();
	}
}
