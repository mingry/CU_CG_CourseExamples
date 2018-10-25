
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <iostream>
#include "Mesh.h"



static const int g_num_shapes=5;
// 0: Box
// 1: Capsule
// 2: Cylinder
// 3: Hemisphere
// 4: Sphere

static Mesh *g_meshes[g_num_shapes];
static GLuint g_vao[g_num_shapes];
static GLuint g_vbo_pos[g_num_shapes];
static GLuint g_vbo_normal[g_num_shapes];
static GLuint g_index_buffer[g_num_shapes];

Mesh g_sphere_mesh;
Mesh g_hemisphere_mesh;
Mesh g_box_mesh;
Mesh g_cylinder_mesh;
Mesh g_capsule_mesh;


void DrawBasicShape(int i)
{
	glBindVertexArray(g_vao[i]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer[i]);
	glDrawElements(GL_TRIANGLES, g_meshes[i]->num_triangles()*3, GL_UNSIGNED_INT, 0);
}

void DrawSphere()
{
	DrawBasicShape(4);
}

void DrawCylinder()
{
	DrawBasicShape(2);
}
void DrawCapsule()
{
	DrawBasicShape(1);
}
void DrawHemisphere()
{
	DrawBasicShape(3);
}
void DrawBox()
{
	DrawBasicShape(0);
}




void InitBasicShapeObjs()
{
	for ( int i=0; i<g_num_shapes; i++ )
	{
		if ( i==0 )			g_box_mesh			.CreateBox();
		else if ( i==1 )	g_capsule_mesh		.CreateCapsule();
		else if ( i==2 )	g_cylinder_mesh		.CreateCylinder();
		else if ( i==3 )	g_hemisphere_mesh	.CreateHemisphere();
		else if ( i==4 )	g_sphere_mesh		.CreateSphere();

		if ( i==0 )			g_meshes[i] = &g_box_mesh;
		else if ( i==1 )	g_meshes[i] = &g_capsule_mesh;
		else if ( i==2 )	g_meshes[i] = &g_cylinder_mesh;
		else if ( i==3 )	g_meshes[i] = &g_hemisphere_mesh;
		else if ( i==4 )	g_meshes[i] = &g_sphere_mesh;


		g_meshes[i]->UpdateNormalVectorsBasedOnVertex();
		g_meshes[i]->TriangulateAndUniformizeBuffers();




		// Vertex Array Object
		glGenVertexArrays(1, &g_vao[i]);
		glBindVertexArray(g_vao[i]);


		// Position VBO
		glGenBuffers(1, &g_vbo_pos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo_pos[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_meshes[i]->num_points(), g_meshes[i]->points(), GL_STATIC_DRAW);


		// Normal VBO
		glGenBuffers(1, &g_vbo_normal[i]);
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*g_meshes[i]->num_points(), g_meshes[i]->normals(), GL_STATIC_DRAW);


		// Vertex Attribute
		glBindBuffer(GL_ARRAY_BUFFER, g_vbo_pos[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal[i]);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);


		// Index Buffer Object
		glGenBuffers(1, &g_index_buffer[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_index_buffer[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*3*g_meshes[i]->num_triangles(), g_meshes[i]->triangle_ids(), GL_STATIC_DRAW);
	}

}



void DeleteBasicShapeObjs()
{
	for ( int i=0; i<g_num_shapes; i++ )
	{
		glDeleteBuffers(1, &g_vbo_pos[i]);
		glDeleteBuffers(1, &g_vbo_normal[i]);
		glDeleteBuffers(1, &g_index_buffer[i]);
		glDeleteVertexArrays(1, &g_vao[i]);
	}

}