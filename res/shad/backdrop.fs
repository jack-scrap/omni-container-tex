#version 400

vec3 beige = vec3(235, 226, 202);

void main() {
	gl_FragColor = vec4(beige / vec3(225.0), 1.0);
}
