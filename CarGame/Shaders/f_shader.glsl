#version 330

in vec4 fs_color;
in vec3 fs_normal;


layout (location = 0) out vec4 color;

uniform vec3 light_dir;	// Directional Light ºûÀÇ ¹æÇâ.


void main()
{
	vec3 N = normalize(fs_normal);
	vec3 L = normalize(-light_dir);

	float diffuse = clamp(dot(L, N), 0.2f, 1.f);

	color = vec4(diffuse, diffuse, diffuse, 1.f) * fs_color;
}