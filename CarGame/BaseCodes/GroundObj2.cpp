
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>

GLuint g_ground2_vao;
GLuint g_ground2_vbo_pos;
GLuint g_ground2_vbo_color;
GLuint g_ground2_vbo_normal;
GLuint g_ground2_index_buffer;
const GLint g_ground2_vertex_num = 4;

void InitGround2()
{
	float positions[g_ground2_vertex_num*3] = 
	{ 5.f, 0.f, 5.f, 
		5.f, 0.f, -5.f,
		-5.f, 0.f, -5.f,
		-5.f, 0.f, 5.f };

	float normals[g_ground2_vertex_num*3] = 
	{ 0.f, 1.f, 0.f, 
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f };

	float colors[g_ground2_vertex_num*4] = 
	{ 1.f, 0.7f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		1.f, 0.7f, 0.f, 1.f,
		0.f, 0.7f, 1.f, 1.f };

	unsigned int indices[6] = {0, 1, 2,  2, 3, 0};




	// Vertex Array Object
	glGenVertexArrays(1, &g_ground2_vao);
	glBindVertexArray(g_ground2_vao);

	// Position VBO
	glGenBuffers(1, &g_ground2_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground2_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*g_ground2_vertex_num*3, positions, GL_STATIC_DRAW);

	// Color VBO
	glGenBuffers(1, &g_ground2_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground2_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_ground2_vertex_num*4, colors, GL_STATIC_DRAW);

	// Normal VBO
	glGenBuffers(1, &g_ground2_vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground2_vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_ground2_vertex_num*3, normals, GL_STATIC_DRAW);



	// Index Buffer Object
	glGenBuffers(1, &g_ground2_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ground2_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indices, GL_STATIC_DRAW);


	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_ground2_vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_ground2_vbo_color);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, g_ground2_vbo_normal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

}


void DrawGround2()
{
	glBindVertexArray(g_ground2_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ground2_index_buffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}


void DeleteGround2()
{
	glDeleteBuffers(1, &g_ground2_vbo_pos);
	glDeleteBuffers(1, &g_ground2_vbo_color);
	glDeleteBuffers(1, &g_ground2_vbo_normal);
	glDeleteBuffers(1, &g_ground2_index_buffer);
	glDeleteVertexArrays(1, &g_ground2_vao);
}