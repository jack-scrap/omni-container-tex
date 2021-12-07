#version 400

in vec3 pos;

uniform mat4
	model,
	view,
	proj;

out vec3 _pos;

void main() {
	gl_Position = proj * view * model * vec4(pos + (normalize(pos) * vec3(0.1)), 1.0);
}
