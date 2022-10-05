#version 330

// Gooch Shaing
// Realtime Rendering 4th Edition Chaper 5.1

in vec4 fs_color;
in vec3 fs_normal;
in vec3 fs_eye_dir;

layout (location = 0) out vec4 color;

uniform vec3 light_dir;	// Directional Light ºûÀÇ ¹æÇâ.


void main()
{
	vec3 L = normalize(-light_dir);
	vec3 N = normalize(fs_normal);
	vec3 V = normalize(-fs_eye_dir);
	

	vec3 highlight = vec3(1.f, 1.f, 1.f);
	vec3 cool = vec3(0.f, 0.f, 0.55f) + 0.25f * fs_color.rgb;
	vec3 warm = vec3(0.3f, 0.3f, 0.f) + 0.25f * fs_color.rgb;
	

	float t = (dot(L, N) + 1.f) / 2.f;
	vec3 R = 2.f*dot(N, L)*N - L;
	float s = clamp(100.f*dot(R, V)-97.f, 0.f, 1.f);

	color.rgb = s*highlight + (1.f-s)*(t*warm+(1.f-t)*cool);
	color.a = fs_color.a;
}