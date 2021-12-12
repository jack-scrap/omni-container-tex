#version 400

const float stroke = 0.16;

in vec3 pos;

out vec3 _pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * model * vec4(pos + (normalize(pos) * vec3(stroke)), 1.0);
}
