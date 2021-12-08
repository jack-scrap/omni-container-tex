#define STB_IMAGE_IMPLEMENTATION

#define VTX_SZ 3
#define ST_SZ 2

#include <vector>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"
#include "stb_image.h"

int main() {
	Disp disp("asdf", 800, 600);

	glm::vec3 scale = glm::vec3(0.02, 0.02, 0.02);

	std::string modelName = "bonk";
	std::string texName = "c_energydrink_blu";

	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<GLfloat> vtcRaw = util::mesh::rd::vtc("res/obj/" + modelName);
	std::vector<GLushort> vtcIdc = util::mesh::rd::idc("res/obj/" + modelName, 0);

	std::vector<GLfloat> vtc;
	for (int i = 0; i < vtcIdc.size(); i++) {
		for (int a = 0; a < VTX_SZ; a++) {
			vtc.push_back(vtcRaw[(vtcIdc[i] * VTX_SZ) + a]);
		}
	}
	glBufferData(GL_ARRAY_BUFFER, vtc.size() * sizeof (GLfloat), &vtc[0], GL_STATIC_DRAW);

	// texture coordinate
	GLuint stbo;
	glGenBuffers(1, &stbo);
	glBindBuffer(GL_ARRAY_BUFFER, stbo);

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
	std::vector<GLushort> idc = util::mesh::rd::idc("res/obj/c_shotgun", 0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idc.size() * sizeof (GLfloat), &idc[0], GL_STATIC_DRAW);

	// matrix
	glm::mat4 model = glm::mat4(1.0);
	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

	// shader
	Prog prog("dir", "dir");

	/// attribute
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	glBindBuffer(GL_ARRAY_BUFFER, stbo);
	GLint attrSt = glGetAttribLocation(prog._id, "st");
	glVertexAttribPointer(attrSt, ST_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrSt);

	// initialize
	prog.use();

	GLint uniModel = glGetUniformLocation(prog._id, "model");
	GLint uniView = glGetUniformLocation(prog._id, "view");
	GLint uniProj = glGetUniformLocation(prog._id, "proj");

	GLint uniTex = glGetUniformLocation(prog._id, "tex");

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	int w;
	int h;
	int c;
	unsigned char* data = stbi_load(std::string("res/tex/" + texName + ".png").c_str(), &w, &h, &c, 3);
	if (!data) {
		std::cout << "Error: Couldn't load" << std::endl;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Prog progOutline("outline", "outline");

	/// attribute
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint attrPosOutline = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPosOutline, VTX_SZ, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPosOutline);

	progOutline.use();

	GLint uniModelOutline = glGetUniformLocation(progOutline._id, "model");
	GLint uniViewOutline = glGetUniformLocation(progOutline._id, "view");
	GLint uniProjOutline = glGetUniformLocation(progOutline._id, "proj");

	glUniformMatrix4fv(uniProjOutline, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniViewOutline, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniModelOutline, 1, GL_FALSE, glm::value_ptr(model));

	SDL_Event e;
	unsigned int t = 0;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}

			if (e.type == SDL_MOUSEWHEEL) {
				if (e.wheel.y > 0) {
					if (glm::all(glm::lessThan(scale, glm::vec3(5.0, 5.0, 5.0)))) {
						scale += glm::vec3(0.1, 0.1, 0.1);
					}
				}

				if (e.wheel.y < 0) {
					if (glm::all(glm::greaterThan(scale, glm::vec3(0.5, 0.5, 0.5)))) {
						scale -= glm::vec3(0.1, 0.1, 0.1);
					}
				}
			}
		}

		disp.clear(42 / 255.0, 39 / 255.0, 37 / 255.0, 1);

		model = glm::mat4(1.0);
		model = glm::rotate(model, (GLfloat) (t * (M_PI / 100.0)), glm::vec3(0, 1, 0));
		model = glm::scale(model, scale);

		glDisable(GL_DEPTH_TEST);

		progOutline.use();

		glUniformMatrix4fv(uniModelOutline, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, idc.size());

		progOutline.unUse();

		glEnable(GL_DEPTH_TEST);

		prog.use();

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, idc.size());

		prog.unUse();

		disp.update();

		t++;
	}
}
