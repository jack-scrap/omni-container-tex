#version 400

in vec3 _pos;

vec3 sun = vec3(3.0, 3.0, 3.0);

void main() {
	vec3 normFace = normalize(cross(dFdx(_pos), dFdy(_pos)));

	float ins = max(dot(normFace, normalize(sun)), 0.0);

	gl_FragColor = vec4(vec3(ins), 1.0);
}