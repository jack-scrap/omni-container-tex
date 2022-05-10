#version 400

in vec3 pos;
in vec2 st;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 _pos;
out vec2 _st;

void main() {
	gl_Position = proj * view * model * vec4(pos, 1.0);

	_pos = gl_Position.xyz;
	_st = st * vec2(1, -1);
}
