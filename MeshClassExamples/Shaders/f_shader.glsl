#version 330

in vec4 fs_color;
in vec3 fs_normal;

uniform int shading_mode = 1;

layout (location = 0) out vec4 color;

void main()
{
	vec3 N = normalize(fs_normal);

	vec3 L = vec3(1.f, 1.f, 1.f);
	L = normalize(L);

	float diffuse = clamp(dot(L, N), 0.2f, 1.f);

	if (shading_mode == 1)
		color = vec4(diffuse, diffuse, diffuse, 1.f) * fs_color;
	else if (shading_mode == 2)
		color = vec4(diffuse, diffuse, diffuse, 1.f);
}