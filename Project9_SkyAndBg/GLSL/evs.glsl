#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;;
out vec3 varyingVertPos;
out vec3 varyingNormal;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
layout (binding = 0) uniform samplerCube tex_map;

void main(void)
{
	varyingVertPos = (mv_matrix * vec4(position, 1.0)).xyz;
	varyingNormal = normalize(vec3(norm_matrix * vec4(normal, 0.0)));
	// calculate the clip space position of the vertex
	gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
}
