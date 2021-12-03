
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include "IL/ilu.h"

extern GLuint s_program_id;

///////////////////////////////////////////////
// Textures
static ILuint g_image;
static unsigned int g_texture_id;
static void LoadTexture();

///////////////////////////////////////////////
// VAO and VBO

GLuint g_ground_t_vao;
GLuint g_ground_t_vbo_pos;
GLuint g_ground_t_vbo_color;
GLuint g_ground_t_vbo_normal;
GLuint g_ground_t_vbo_uv;
GLuint g_ground_t_index_buffer;
const GLint g_ground_t_vertex_num = 4;

void InitGroundTexture()
{
	LoadTexture();

	float positions[g_ground_t_vertex_num*3] = 
		{ 5.f, 0.f, 5.f, 
		5.f, 0.f, -5.f,
		-5.f, 0.f, -5.f,
		-5.f, 0.f, 5.f };

	float normals[g_ground_t_vertex_num*3] = 
		{ 0.f, 1.f, 0.f, 
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f };

	float colors[g_ground_t_vertex_num*4] = 
		{ 1.f, 0.7f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,
		1.f, 0.7f, 0.f, 1.f,
		0.f, 0.7f, 1.f, 1.f };

	float uvs[g_ground_t_vertex_num*2] = 
		{ 0.f, 0.0f, 
		0.f, 1.f,
		1.f, 1.f, 
		1.f, 0.f };


	
	unsigned int indices[6] = {0, 1, 2,  2, 3, 0};

	


	// Vertex Array Object
	glGenVertexArrays(1, &g_ground_t_vao);
	glBindVertexArray(g_ground_t_vao);

	// Position VBO
	glGenBuffers(1, &g_ground_t_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*g_ground_t_vertex_num*3, positions, GL_STATIC_DRAW);

	// Color VBO
	glGenBuffers(1, &g_ground_t_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_ground_t_vertex_num*4, colors, GL_STATIC_DRAW);

	// Normal VBO
	glGenBuffers(1, &g_ground_t_vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_ground_t_vertex_num*3, normals, GL_STATIC_DRAW);

	// UV VBO
	glGenBuffers(1, &g_ground_t_vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*g_ground_t_vertex_num*2, uvs, GL_STATIC_DRAW);



	// Index Buffer Object
	glGenBuffers(1, &g_ground_t_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ground_t_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*6, indices, GL_STATIC_DRAW);


	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_normal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_color);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, g_ground_t_vbo_uv);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

}



void LoadTexture()
{
	ilGenImages(1, &g_image);
	ilBindImage(g_image);
	iluLoadImage("../Data/road.jpg");
	int img_w = ilGetInteger(IL_IMAGE_WIDTH);		// it must be a number of 2^n;
	int img_h = ilGetInteger(IL_IMAGE_HEIGHT);	// it must be same to the img_w;


	glGenTextures(1, &g_texture_id);
	glBindTexture(GL_TEXTURE_2D, g_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

	glGenerateMipmap(GL_TEXTURE_2D);

}


void DrawGroundTexture()
{
	glBindTexture(GL_TEXTURE_2D, g_texture_id);

	glBindVertexArray(g_ground_t_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ground_t_index_buffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}


void DeleteGroundTexture()
{
	glDeleteBuffers(1, &g_ground_t_vbo_pos);
	glDeleteBuffers(1, &g_ground_t_vbo_color);
	glDeleteBuffers(1, &g_ground_t_vbo_normal);
	glDeleteBuffers(1, &g_ground_t_vbo_uv);
	glDeleteBuffers(1, &g_ground_t_index_buffer);
	glDeleteVertexArrays(1, &g_ground_t_vao);
	ilDeleteImage(g_image);
}


