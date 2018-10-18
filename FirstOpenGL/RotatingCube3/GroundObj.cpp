
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>

static GLuint g_ground_vao;
static GLuint g_ground_vbo_pos;


void InitGround()
{
	const int lineNum = 51;
	float blockWidth = 0.2f;
	float min = -1 * (50 / 2) * blockWidth;
	float max = (50 / 2) * blockWidth;

	glm::vec3 positions[51 * 2 * 2];

	for (int i = 0; i < lineNum; i += 1)
	{
		positions[i * 2 + 0] = glm::vec3(min + i * blockWidth, 0.f, min);
		positions[i * 2 + 1] = glm::vec3(min + i * blockWidth, 0.f, max);

		positions[51 * 2 + i * 2 + 0] = glm::vec3(min, 0.f, min + i * blockWidth);
		positions[51 * 2 + i * 2 + 1] = glm::vec3(max, 0.f, min + i * blockWidth);
	}


	// Vertex Array Object
	glGenVertexArrays(1, &g_ground_vao);
	glBindVertexArray(g_ground_vao);

	// Vertex Buffer Object
	glGenBuffers(1, &g_ground_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 51 * 2 * 2, positions, GL_STATIC_DRAW);


	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_vbo_pos);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

}


void DrawGround()
{
	glLineWidth(1);
	glBindVertexArray(g_ground_vao);
	glVertexAttrib3f(1, 0.0f, 1.f, 0.0f);
	glVertexAttrib4f(2, 0.9f, 0.9f, 0.9f, 1.0f);
	glDrawArrays(GL_LINES, 0, 51 * 2 * 2);
}

void DeleteGround()
{
	glDeleteBuffers(1, &g_ground_vbo_pos);
	glDeleteVertexArrays(1, &g_ground_vao);
}