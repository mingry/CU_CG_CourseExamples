#include "treeModel0.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <iostream>
#include "../BaseCodes/Mesh.h"
#include "../BaseCodes/BasicShapeObjs.h"



///////////////////////////////////////////////
// Tree

static GLuint g_tree_vao;
static GLuint g_tree_vbo_pos;
static GLuint g_tree_vbo_color;
static GLuint g_tree_vbo_normal;
static GLuint g_tree_index_buf;

static Mesh g_tree_mesh;

void InitTreeModel()
{
	// Element 1, ³ª¹«±âµÕ
	Mesh trunk = glm::scale(glm::vec3(0.02f, 0.50f, 0.02f)) 
				* glm::translate(glm::vec3(0.f, 0.5f, 0.f)) 
				* g_cylinder_mesh;
	trunk.SetColor(1.f, 0.f, 1.f, 1.f);
	
	// Element 2, ³ª¹µÀÙ
	Mesh leaf = glm::rotate(glm::pi<float>()/2.f, glm::vec3(0.f, 1.f, 0.f))
				* glm::rotate(-glm::pi<float>()/6.f, glm::vec3(1.f, 0.f, 0.f))
				* glm::scale(glm::vec3(0.05f, 0.002f, 0.15f)) 
				* glm::translate(glm::vec3(0.f, 0.f, 1.f)) 
				* g_cylinder_mesh;
	leaf.SetColor(0.f, 1.f, 0.f, 1.f);



	// Tree, ³ª¹«±âµÕ+³ª¹µÀÓ+³ª¹«±âµÕ
	glm::mat4 T(1.0);
	g_tree_mesh = trunk;
	T = glm::translate(glm::vec3(0.f, 0.50f, 0.f));
	g_tree_mesh += T * leaf;
	T *= glm::rotate(glm::pi<float>()/6, glm::vec3(1.f, 0.f, 0.f));
	g_tree_mesh += T * trunk;






	// Vertex Array Object
	glGenVertexArrays(1, &g_tree_vao);
	glBindVertexArray(g_tree_vao);


	// Position VBO
	glGenBuffers(1, &g_tree_vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_tree_mesh.num_points(), g_tree_mesh.points(), GL_STATIC_DRAW);

	// Color VBO
	glGenBuffers(1, &g_tree_vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*g_tree_mesh.num_points(), g_tree_mesh.colors(), GL_STATIC_DRAW);

	// Normal VBO
	glGenBuffers(1, &g_tree_vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_tree_mesh.num_points(), g_tree_mesh.normals(), GL_STATIC_DRAW);


	// Index Buffer Object
	glGenBuffers(1, &g_tree_index_buf);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tree_index_buf);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_tree_mesh.num_triangles(), g_tree_mesh.triangle_ids(), GL_STATIC_DRAW);


	// Vertex Attribute
	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_pos);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_color);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, g_tree_vbo_normal);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
}


void DrawTreeModel()
{
	glBindVertexArray(g_tree_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_tree_index_buf);
	glDrawElements(GL_TRIANGLES, g_tree_mesh.num_triangles()*3, GL_UNSIGNED_INT, NULL);
}


void DeleteTreeModel()
{
	glDeleteBuffers(1, &g_tree_vbo_pos);
	glDeleteBuffers(1, &g_tree_vbo_color);
	glDeleteBuffers(1, &g_tree_vbo_normal);
	glDeleteBuffers(1, &g_tree_index_buf);
	glDeleteVertexArrays(1, &g_tree_vao);
}




