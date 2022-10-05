#version 330

layout (location = 0) in vec3 vs_position;
layout (location = 1) in vec3 vs_normal;
layout (location = 2) in vec4 vs_color;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out vec3 fs_normal;
out vec4 fs_color;

void main()
{
	gl_Position = proj_matrix * view_matrix * model_matrix * vec4(vs_position, 1.f);
	fs_normal = inverse(transpose(mat3(view_matrix * model_matrix))) * vs_normal;
	fs_color = vs_color;
}