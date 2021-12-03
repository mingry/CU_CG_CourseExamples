
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>
#include <fstream>
#include "../BaseCodes/Mesh.h"
#include "IL/ilu.h"

extern GLuint s_program_id;

///////////////////////////////////////////////
// Textures
static ILuint g_image;
static unsigned int g_texture_id;
static void LoadTexture();

///////////////////////////////////////////////
// VAO and VBO

GLuint g_moon_t_vao;
GLuint g_moon_t_vbo_pos;
GLuint g_moon_t_vbo_color;
GLuint g_moon_t_vbo_normal;
GLuint g_moon_t_vbo_uv;
GLuint g_moon_t_index_buffer;

static Mesh g_moon_mesh;

void InitMoonTexture()
{
	LoadTexture();

	g_moon_mesh.ReadObjFormatFile("../Data/Moon_2K.obj");
	g_moon_mesh.TriangulateAndUniformizeBuffers();

	// Vertex Array Object
	glGenVertexArrays(1, &g_moon_t_vao);
	glBindVertexArray(g_moon_t_vao);

	// Position VBO
	glGenBuffers(1, &g_moon_t_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*g_moon_mesh.num_points(), g_moon_mesh.points(), GL_STATIC_DRAW);

	// Color VBO
	glGenBuffers(1, &g_moon_t_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*g_moon_mesh.num_points(), g_moon_mesh.colors(), GL_STATIC_DRAW);

	// Normal VBO
	glGenBuffers(1, &g_moon_t_vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3* g_moon_mesh.num_points(), g_moon_mesh.normals(), GL_STATIC_DRAW);

	// UV VBO
	glGenBuffers(1, &g_moon_t_vbo_uv);
	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2* g_moon_mesh.num_points(), g_moon_mesh.uvs(), GL_STATIC_DRAW);


	// Index Buffer Object
	glGenBuffers(1, &g_moon_t_index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_moon_t_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_moon_mesh.num_triangles(), g_moon_mesh.triangle_ids(), GL_STATIC_DRAW);


	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_normal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_color);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, g_moon_t_vbo_uv);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

}



void LoadTexture()
{
	ilGenImages(1, &g_image);
	ilBindImage(g_image);
	iluLoadImage("../Data/Diffuse_2K.png");

	// OpenGL에서는 가로 세로 길이가 2의 n승인 이미지를 사용해야하므로
	// iluScale 함수를 이용하여 이미지 크기를 변경한다.
	iluScale(2048, 2048, ilGetInteger(IL_IMAGE_DEPTH));	

	int img_w = ilGetInteger(IL_IMAGE_WIDTH);
	int img_h = ilGetInteger(IL_IMAGE_HEIGHT);

	glGenTextures(1, &g_texture_id);
	glBindTexture(GL_TEXTURE_2D, g_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_w, img_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glGenerateMipmap(GL_TEXTURE_2D);

}


void DrawMoonTexture()
{
	glBindTexture(GL_TEXTURE_2D, g_texture_id);

	glBindVertexArray(g_moon_t_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_moon_t_index_buffer);
	glDrawElements(GL_TRIANGLES, g_moon_mesh.num_triangles()*3, GL_UNSIGNED_INT, NULL);
}


void DeleteMoonTexture()
{
	glDeleteBuffers(1, &g_moon_t_vbo_pos);
	glDeleteBuffers(1, &g_moon_t_vbo_color);
	glDeleteBuffers(1, &g_moon_t_vbo_normal);
	glDeleteBuffers(1, &g_moon_t_vbo_uv);
	glDeleteBuffers(1, &g_moon_t_index_buffer);
	glDeleteVertexArrays(1, &g_moon_t_vao);
	ilDeleteImage(g_image);
}


