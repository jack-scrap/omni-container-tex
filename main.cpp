#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "disp.h"
#include "prog.h"
#include "util.h"
#include "prop.h"
#include "col.h"

extern Disp disp("asdf", 800, 600);

int main() {
	Prop prop("container_2x4", "0");

	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);

	bool pan = false;

	SDL_Event e;
	unsigned int r = 0;

	prop._model = glm::mat4(1.0);
	prop._model = glm::rotate(prop._model, (GLfloat) r, glm::vec3(0, 1, 0));
	prop._model = glm::scale(prop._model, scale);

	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_F1) {
					prop._showWire = !prop._showWire;
				}
			}

			if (e.type == SDL_MOUSEBUTTONDOWN) {
				pan = true;
			}

			if (e.type == SDL_MOUSEBUTTONUP) {
				pan = false;
			}

			if (e.type == SDL_MOUSEMOTION) {
				if (pan) {
					r++;

					prop._model = glm::mat4(1.0);
					prop._model = glm::rotate(prop._model, (GLfloat) (r * (M_PI / 100.0)), glm::vec3(0, 1, 0));
					prop._model = glm::scale(prop._model, scale);
				}
			}

			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}
		}

		disp.clear(col[false].r / 255.0, col[false].g / 255.0, col[false].b / 255.0, 1);

		prop.draw();

		disp.update();
	}
}
