#version 330

layout (location = 0) in vec3 vs_position;	// 꼭지점 위치
layout (location = 1) in vec3 vs_normal;	// 꼭지점 노말벡터
layout (location = 2) in vec4 vs_color;		// 꼭지점 색깔

uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out vec3 fs_normal;	// 변환 행렬이 적용된 노말벡터.
out vec4 fs_color;
out vec3 fs_eye_dir;	// 보는 대상 (꼭지점)이 있는 곳으로 향하는 벡터.


void main()
{
	mat4 modelview = view_matrix * model_matrix;

	gl_Position = proj_matrix * modelview * vec4(vs_position, 1.f);
	fs_normal = vec3(modelview * vec4(vs_normal, 0.f));	// 노말벡터에는 이동 변환(Tanslation)을 적용하지 않음.
	fs_eye_dir = vec3(modelview * vec4(vs_position, 1.f));	// Projection을 적용하지 않음.

	fs_color = vs_color;
}

