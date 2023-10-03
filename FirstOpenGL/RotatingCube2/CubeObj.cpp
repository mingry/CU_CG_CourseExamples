

#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>

//////////////////////////////////////////////////////////////////////
//// Define Mesh Model
//////////////////////////////////////////////////////////////////////
static const int g_num_vertices = 24;
static const int g_num_triangles = 12;

//// Declare VAO, VBO variables
//////////////////////////////////////////////////////////////////////
static GLuint g_vao_id;
static GLuint g_vbo_position_id;
static GLuint g_vbo_normal_id;
static GLuint g_vbo_color_id;
static GLuint g_index_buffer_id;

// Index Buffer Object
static GLuint index_buffer_id;

void InitCube()
{
	static GLfloat m_positions[g_num_vertices * 3] =
	{ 0.25f,  0.25f,  0.25f,
	  -0.25f,  0.25f,  0.25f,
	  -0.25f, -0.25f,  0.25f,
	   0.25f, -0.25f,  0.25f,

	   0.25f,  0.25f,  0.25f,
	   0.25f,  0.25f, -0.25f,
	  -0.25f,  0.25f, -0.25f,
	  -0.25f,  0.25f,  0.25f,

	   0.25f,  0.25f,  0.25f,
	   0.25f, -0.25f,  0.25f,
	   0.25f, -0.25f, -0.25f,
	   0.25f,  0.25f, -0.25f,

	   0.25f,  0.25f, -0.25f,
	   0.25f, -0.25f, -0.25f,
	  -0.25f, -0.25f, -0.25f,
	  -0.25f,  0.25f, -0.25f,

	   0.25f, -0.25f,  0.25f,
	  -0.25f, -0.25f,  0.25f,
	  -0.25f, -0.25f, -0.25f,
	   0.25f, -0.25f, -0.25f,

	  -0.25f,  0.25f,  0.25f,
	  -0.25f,  0.25f, -0.25f,
	  -0.25f, -0.25f, -0.25f,
	  -0.25f, -0.25f,  0.25f
	};

	static  GLfloat m_normals[g_num_vertices * 3] =
	{ 0.f,  0.f,  1.f,
	   0.f,  0.f,  1.f,
	   0.f,  0.f,  1.f,
	   0.f,  0.f,  1.f,

	   0.f,  1.f,  0.f,
	   0.f,  1.f,  0.f,
	   0.f,  1.f,  0.f,
	   0.f,  1.f,  0.f,

	   1.f,  0.f,  0.f,
	   1.f,  0.f,  0.f,
	   1.f,  0.f,  0.f,
	   1.f,  0.f,  0.f,

	   0.f,  0.f, -1.f,
	   0.f,  0.f, -1.f,
	   0.f,  0.f, -1.f,
	   0.f,  0.f, -1.f,

	   0.f, -1.f,  0.f,
	   0.f, -1.f,  0.f,
	   0.f, -1.f,  0.f,
	   0.f, -1.f,  0.f,

	  -1.f,  0.f,  0.f,
	  -1.f,  0.f,  0.f,
	  -1.f,  0.f,  0.f,
	  -1.f,  0.f,  0.f
	};

	static GLfloat m_colors[g_num_vertices * 4] =
	{ 1.f, 0.f, 0.f, 1.f,
	  0.f, 1.f, 0.f, 1.f,
	  0.f, 0.f, 1.f, 1.f,
	  1.f, 1.f, 0.f, 1.f,

	  1.f, 0.f, 0.f, 1.f,
	  0.f, 1.f, 0.f, 1.f,
	  0.f, 0.f, 1.f, 1.f,
	  1.f, 1.f, 0.f, 1.f,

	  1.f, 0.f, 0.f, 1.f,
	  0.f, 1.f, 0.f, 1.f,
	  0.f, 0.f, 1.f, 1.f,
	  1.f, 1.f, 0.f, 1.f,

	  1.f, 0.f, 0.f, 1.f,
	  0.f, 1.f, 0.f, 1.f,
	  0.f, 0.f, 1.f, 1.f,
	  1.f, 1.f, 0.f, 1.f,

	  1.f, 0.f, 0.f, 1.f,
	  0.f, 1.f, 0.f, 1.f,
	  0.f, 0.f, 1.f, 1.f,
	  1.f, 1.f, 0.f, 1.f,

	  1.f, 0.f, 0.f, 1.f,
	  0.f, 1.f, 0.f, 1.f,
	  0.f, 0.f, 1.f, 1.f,
	  1.f, 1.f, 0.f, 1.f
	};

	static GLuint m_indices[g_num_triangles * 3] =
	{
	   0, 1, 2,
	   0, 2, 3,

	   4, 5, 6,
	   4, 6, 7,

	   8,  9, 10,
	   8, 10, 11,

	  12, 13, 14,
	  12, 14, 15,

	  16, 17, 18,
	  16, 18, 19,

	  20, 21, 22,
	  20, 22, 23
	};

	//// 5.1. VAO 객체 생성 및 바인딩
	glGenVertexArrays(1, &g_vao_id);
	glBindVertexArray(g_vao_id);


	//// 5.2. vertex positions 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &g_vbo_position_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_position_id);

	//// 5.3. vertex positions 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * g_num_vertices, m_positions, GL_STATIC_DRAW);

	//// 5.4. 현재 바인딩되어있는 VBO를 shader program과 연결
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	//// 5.5. vertex normal vectors 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &g_vbo_normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal_id);

	//// 5.6. vertex positions 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * g_num_vertices, m_normals, GL_STATIC_DRAW);

	//// 5.7. 현재 바인딩되어있는 VBO를 shader program과 연결
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	//// 5.8. vertex colors 저장을 위한 VBO 생성 및 바인딩.
	glGenBuffers(1, &g_vbo_color_id);
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_color_id);

	//// 5.9. vertex positions 데이터 입력.
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * g_num_vertices, m_colors, GL_STATIC_DRAW);

	//// 5.10. 현재 바인딩되어있는 VBO를 shader program과 연결
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);


	//// 5.11. Index Buffer 객체 생성 및 데이터 입력
	glGenBuffers(1, &g_index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * g_num_triangles, m_indices, GL_STATIC_DRAW);
}


void DrawCube()
{
	glBindVertexArray(g_vao_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer_id);
	glDrawElements(GL_TRIANGLES, g_num_triangles * 3, GL_UNSIGNED_INT, 0);
}


void DeleteCube()
{
	//// 5.12. VAO, VBO 메모리 해제. 
	glDeleteBuffers(1, &g_vbo_position_id);
	glDeleteBuffers(1, &g_vbo_color_id);
	glDeleteBuffers(1, &g_vbo_normal_id);
	glDeleteVertexArrays(1, &g_vao_id);
}