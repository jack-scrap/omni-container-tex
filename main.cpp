#include <vector>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"

int main() {
	Disp disp("asdf", 800, 600);

	glm::vec3 scale = glm::vec3(0.2, 0.2, 0.2);

	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	std::vector<GLfloat> vtc = util::mesh::rd::vtc("c_shotgun");
	glBufferData(GL_ARRAY_BUFFER, vtc.size() * sizeof (GLfloat), &vtc[0], GL_STATIC_DRAW);

	// texture coordinate
	GLuint stbo;
	glGenBuffers(1, &stbo);
	glBindBuffer(GL_ARRAY_BUFFER, stbo);

	std::vector<GLfloat> st = util::mesh::rd::st("c_shotgun");
	glBufferData(GL_ARRAY_BUFFER, st.size() * sizeof (GLfloat), &st[0], GL_STATIC_DRAW);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	std::vector<GLushort> idc = util::mesh::rd::idc("c_shotgun", 0);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idc.size() * sizeof (GLfloat), &idc[0], GL_STATIC_DRAW);

	// matrix
	glm::mat4
		model = glm::mat4(1.0),
		view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
		proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

	// shader
	Prog prog("dir", "dir");

	/// attribute
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	glBindBuffer(GL_ARRAY_BUFFER, stbo);
	GLint attrSt = glGetAttribLocation(prog._id, "st");
	glVertexAttribPointer(attrSt, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrSt);

	// initialize
	prog.use();

	GLint
		uniModel = glGetUniformLocation(prog._id, "model"),
	 	uniView = glGetUniformLocation(prog._id, "view"),
	 	uniProj = glGetUniformLocation(prog._id, "proj");

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Prog progOutline("outline", "outline");

	/// attribute
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	GLint attrPosOutline = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPosOutline, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPosOutline);

	progOutline.use();

	GLint
		uniModelOutline = glGetUniformLocation(progOutline._id, "model"),
		uniViewOutline = glGetUniformLocation(progOutline._id, "view"),
		uniProjOutline = glGetUniformLocation(progOutline._id, "proj");

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

		glDrawElements(GL_TRIANGLES, idc.size(), GL_UNSIGNED_SHORT, (GLvoid*) 0);

		progOutline.unUse();

		glEnable(GL_DEPTH_TEST);

		prog.use();

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glDrawElements(GL_TRIANGLES, idc.size(), GL_UNSIGNED_SHORT, (GLvoid*) 0);

		prog.unUse();

		disp.update();

		t++;
	}
}
