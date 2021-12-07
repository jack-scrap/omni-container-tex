#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"

int main() {
	Disp disp("asdf", 800, 600);

	// data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vtc[3 * 2 * 2 * 3 * 3] = {
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,

		-1.0, 1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, 1.0, -1.0,

		// front
		-1.0, -1.0, 1.0,
		1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,

		-1.0, 1.0, 1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,

		// left
		-1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, -1.0,

		-1.0, 1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, 1.0, 1.0,

		// right
		1.0, -1.0, -1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, -1.0,

		1.0, 1.0, -1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,

		// bottom
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		-1.0, -1.0, 1.0,

		-1.0, -1.0, 1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0, 1.0,

		// top
		-1.0, 1.0, -1.0,
		1.0, 1.0, -1.0,
		-1.0, 1.0, 1.0,

		-1.0, 1.0, 1.0,
		1.0, 1.0, -1.0,
		1.0, 1.0, 1.0
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof vtc, vtc, GL_STATIC_DRAW);

	// matrix
	glm::mat4
		model = glm::mat4(1.0),
		view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
		proj = glm::perspective(glm::radians(45.0), 800.0 / 600.0, 0.1, 100.0);

	// shader
	Prog prog("shad", "shad");

	/// attribute
	GLint attrPos = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPos, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPos);

	/// uniform
	GLint
		uniModel = glGetUniformLocation(prog._id, "model"),
	 	uniView = glGetUniformLocation(prog._id, "view"),
	 	uniProj = glGetUniformLocation(prog._id, "proj");

	// initialize
	prog.use();

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	Prog progOutline("outline", "outline");

	/// attribute
	GLint attrPosOutline = glGetAttribLocation(prog._id, "pos");
	glVertexAttribPointer(attrPosOutline, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*) 0);
	glEnableVertexAttribArray(attrPosOutline);

	/// uniform
	GLint
		uniModelOutline = glGetUniformLocation(progOutline._id, "model"),
	 	uniViewOutline = glGetUniformLocation(progOutline._id, "view"),
	 	uniProjOutline = glGetUniformLocation(progOutline._id, "proj");

	progOutline.use();

	glUniformMatrix4fv(uniProjOutline, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniViewOutline, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(uniModelOutline, 1, GL_FALSE, glm::value_ptr(model));

	SDL_Event e;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(0, 0, 0, 1);

		model = glm::rotate(model, (GLfloat) (M_PI / 100.0), glm::vec3(0, 1, 0));

		glDisable(GL_DEPTH_TEST);

		progOutline.use();

		glUniformMatrix4fv(uniModelOutline, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 2 * 3);

		progOutline.unUse();

		glEnable(GL_DEPTH_TEST);

		prog.use();

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 2 * 3);

		prog.unUse();

		disp.update();
	}
}
