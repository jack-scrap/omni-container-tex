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
#include "stb_image.h"
#include "prop.h"
#include "col.h"

int main() {
	Disp disp("asdf", 800, 600);

	Prop prop("c_bonk", "c_energydrink_blu");

	glm::vec3 scale = glm::vec3(0.02, 0.02, 0.02);

	SDL_Event e;
	unsigned int t = 0;
	while (disp.open) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				SDL_Quit();
			}

			if (e.type == SDL_MOUSEWHEEL) {
				if (e.wheel.y > 0) {
					if (glm::all(glm::lessThan(scale, glm::vec3(0.05, 0.05, 0.05)))) {
						scale += glm::vec3(0.01, 0.01, 0.01);
					}
				}

				if (e.wheel.y < 0) {
					if (glm::all(glm::greaterThan(scale, glm::vec3(0.01, 0.01, 0.01)))) {
						scale -= glm::vec3(0.01, 0.01, 0.01);
					}
				}
			}
		}

		disp.clear(bg.r / 255.0, bg.g / 255.0, bg.b / 255.0, 1);

		prop._model = glm::mat4(1.0);
		prop._model = glm::scale(prop._model, scale);

		prop.draw();

		disp.update();

		t++;
	}
}
