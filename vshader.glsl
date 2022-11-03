#version 460

layout (location=0) in vec3 position;
layout (location=1) in vec2 tcs;
layout (location=2) in vec3 normal;

out vec2 tc;
out vec3 fPosition;
out vec3 fNormal;
out vec3 light_dir;

void main()
{
	vec3 light_pos = vec3(100.0, 100.0, 100.0);

	gl_Position = vec4(position, 1.0) + vec4(0.0, 0.0, -8.0, 0.0);
	gl_Position *= vec4(0.5, 0.5, 0.5, 1.0);
	fPosition = gl_Position.xyz;
	fNormal = normal - vec3(0.0, 0.0, -8.0);
	light_dir = light_pos - fPosition;
	tc = tcs;
}
