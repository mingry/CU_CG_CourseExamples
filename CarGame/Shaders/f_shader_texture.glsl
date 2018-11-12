#version 330

in vec4 fs_color;
in vec2 fs_uv;
in vec3 fs_normal;

uniform int shading_mode = 1;

layout (location = 0) out vec4 color;

uniform sampler2D tex0;
uniform bool flag_texture;

void main()
{
	vec3 N = normalize(fs_normal);
	vec3 L = vec3(1.f, 1.f, 1.f);
	L = normalize(L);

	float diffuse = clamp(dot(L, N), 0.2f, 1.f);

	if (flag_texture)
		color = vec4(diffuse, diffuse, diffuse, 1.f) * texture(tex0, fs_uv);
	else 
		color = vec4(diffuse, diffuse, diffuse, 1.f) * fs_color;
}