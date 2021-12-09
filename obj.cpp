#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "obj.h"
#include "stb_image.h"

Obj::Obj(std::string modelName, std::string texName) :
	_prog("res/shad/dir", "res/shad/dir"),
	_progOutline("res/shad/outline", "res/shad/outline") {
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

		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		std::vector<GLushort> idc = util::mesh::rd::idc("res/obj/c_shotgun", 0);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, idc.size() * sizeof (GLfloat), &idc[0], GL_STATIC_DRAW);

		_model = glm::mat4(1.0);
		_view = glm::lookAt(glm::vec3(3.0, 3.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0, 1, 0));
		_proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		_attrPos = glGetAttribLocation(_prog._id, "pos");
		glVertexAttribPointer(_attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(_attrPos);

		glBindBuffer(GL_ARRAY_BUFFER, _stbo);
		_attrSt = glGetAttribLocation(_prog._id, "st");
		glVertexAttribPointer(_attrSt, ST_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(_attrSt);

		_prog.use();

		_uniModel = glGetUniformLocation(_prog._id, "model");
		_uniView = glGetUniformLocation(_prog._id, "view");
		_uniProj = glGetUniformLocation(_prog._id, "proj");

		_uniTex = glGetUniformLocation(_prog._id, "tex");

		_tex;
		glGenTextures(1, &_tex);
		glBindTexture(GL_TEXTURE_2D, _tex);

		int wd;
		int ht;
		int chan;
		unsigned char* data = stbi_load(std::string("res/tex/" + texName + ".png").c_str(), &wd, &ht, &chan, 3);
		if (!data) {
			std::cout << "Error: Couldn't load" << std::endl;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wd, ht, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);

		glUniformMatrix4fv(_uniProj, 1, GL_FALSE, glm::value_ptr(_proj));
		glUniformMatrix4fv(_uniView, 1, GL_FALSE, glm::value_ptr(_view));
		glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(_model));

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		GLint attrPosOutline = glGetAttribLocation(_prog._id, "pos");
		glVertexAttribPointer(attrPosOutline, VTX_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
		glEnableVertexAttribArray(attrPosOutline);

		_progOutline.use();

		_uniModelOutline = glGetUniformLocation(_progOutline._id, "model");
		_uniViewOutline = glGetUniformLocation(_progOutline._id, "view");
		_uniProjOutline = glGetUniformLocation(_progOutline._id, "proj");

		glUniformMatrix4fv(_uniProjOutline, 1, GL_FALSE, glm::value_ptr(_proj));
		glUniformMatrix4fv(_uniViewOutline, 1, GL_FALSE, glm::value_ptr(_view));
		glUniformMatrix4fv(_uniModelOutline, 1, GL_FALSE, glm::value_ptr(_model));
	}

void Obj::draw() {
	glDisable(GL_DEPTH_TEST);

	_progOutline.use();

	glUniformMatrix4fv(_uniModelOutline, 1, GL_FALSE, glm::value_ptr(_model));

	glDrawArrays(GL_TRIANGLES, 0, _noEl);

	_progOutline.unUse();

	glEnable(GL_DEPTH_TEST);

	_prog.use();

	glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(_model));

	glDrawArrays(GL_TRIANGLES, 0, _noEl);

	_prog.unUse();
}
