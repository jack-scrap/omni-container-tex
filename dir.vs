#version 400

in vec3 pos;
in vec2 st;

uniform mat4
	model,
	view,
	proj;

out vec3 _pos;
out vec2 _st;

void main() {
	gl_Position = proj * view * model * vec4(pos, 1.0);

	_pos = pos;
	_st = st;
}
