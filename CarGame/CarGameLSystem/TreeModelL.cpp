#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>
#include "../BaseCodes/Mesh.h"
#include "LSystem.h"
#include "TreeModelL.h"



///////////////////////////////////////////////
// Trees


GLuint g_tree_vao[NUM_TREES];
GLuint g_tree_vbo_pos[NUM_TREES];
GLuint g_tree_vbo_color[NUM_TREES];
GLuint g_tree_vbo_normal[NUM_TREES];
GLuint g_tree_index_buf[NUM_TREES];

static Mesh g_tree_mesh[NUM_TREES];

void InitTreeModel()
{
	for ( int i=0; i<NUM_TREES; i++ )
	{

		CreateLSystemTree(i+1, g_tree_mesh[i]);


		// Vertex Array Object
		glGenVertexArrays(1, &g_tree_vao[i]);
		glBindVertexArray(g_tree_vao[i]);


		// Position VBO
		glGenBuffers(1, &g_tree_vbo_pos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_pos[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_tree_mesh[i].num_points(), g_tree_mesh[i].points(), GL_STATIC_DRAW);

		// Color VBO
		glGenBuffers(1, &g_tree_vbo_color[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_color[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*g_tree_mesh[i].num_points(), g_tree_mesh[i].colors(), GL_STATIC_DRAW);

		// Normal VBO
		glGenBuffers(1, &g_tree_vbo_normal[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_tree_mesh[i].num_points(), g_tree_mesh[i].normals(), GL_STATIC_DRAW);


		// Index Buffer Object
		glGenBuffers(1, &g_tree_index_buf[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tree_index_buf[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_tree_mesh[i].num_triangles(), g_tree_mesh[i].triangle_ids(), GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_pos[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_normal[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_color[i]);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

	}
}


void DrawTreeModel(int i)
{
	glBindVertexArray(g_tree_vao[i]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tree_index_buf[i]);
	glDrawElements(GL_TRIANGLES, g_tree_mesh[i].num_triangles()*3, GL_UNSIGNED_INT, NULL);
}


void DeleteTreeModel()
{
	for ( int i=0; i<NUM_TREES; i++ )
	{
		glDeleteBuffers(1, &g_tree_vbo_pos[i]);
		glDeleteBuffers(1, &g_tree_vbo_color[i]);
		glDeleteBuffers(1, &g_tree_vbo_normal[i]);
		glDeleteBuffers(1, &g_tree_index_buf[i]);
		glDeleteVertexArrays(1, &g_tree_vao[i]);
	}
}




