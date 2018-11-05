
#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <stdarg.h>
#include <string>
#include <vector>

///////////////////////////////////////////
// Class: Mesh

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh &in_mesh);
	~Mesh();
	void Clear();

	// Create Premitives.
	void CreateBox(float width=1, float height=1, float depth=1);
	void CreateCylinder(glm::vec3 bottom_center, glm::vec3 top_center, float radius=1, int slices=32, int stacks=1);
	void CreateCylinder(float height=1, float radius=1, int slices=16, int stacks=1);
	void CreateOpenedCylinder(float height=1, float radius=1, int slices=16, int stacks=1);
	void CreateSphere(float radius=1);
	void CreateHemisphere(float radius=1);
	void CreateCapsule(float cylinder_height=1, float radius=1);

	// Transform.
	void TranslateVertices(glm::vec3 t);
	void RotateVertices(glm::quat q, glm::vec3 center=glm::vec3(0, 0, 0));
	void RotateVertices(float angle, glm::vec3 axis);
	void ScaleUniformlyVertices(float s, glm::vec3 center=glm::vec3(0, 0, 0));
	void ScaleVertices(glm::vec3 s, glm::vec3 center=glm::vec3(0, 0, 0));
	void TransformVertices(const glm::mat4 &m);

	// Assign, Merge
	void Assign(const Mesh& mesh);
	void Merge(const Mesh& mesh);

	// Calculate Normal Vector.
	glm::vec3 CalculFaceNormal(int face_id) const;
	glm::vec3 CalculVertexNormal(int vertex_id) const;
	void UpdateNormalVectorsBasedOnFace();
	void UpdateNormalVectorsBasedOnVertex();

	//
	void TriangulateAndUniformizeBuffers();

	
	// Get Properties. 
	inline int num_points() const { return (int)points_.size(); }
	inline int num_uvs() const { return (int)uvs_.size(); }
	inline int num_normals() const { return (int)normals_.size(); }
	inline int num_triangles() const { return (int)triangle_ids_.size(); }
	inline int num_faces() const { return (int)face_sizes_.size(); }
	
	inline float*  points() const { return (float*)points_.data(); }
	inline float*  colors() const { return (float*)colors_.data(); }
	inline float*  uvs() const { return (float*)uvs_.data(); }
	inline float*  normals() const { return (float*)normals_.data(); }

	inline glm::vec2 uv(int id) const { return uvs_[id]; }
	inline glm::vec3 point(int id)              const { return points_[id]; }
	inline glm::vec3 normal(int id)              const { return normals_[id]; }

	/**
	It will be available only after calling TriangulateAndUniformizeBuffers().
	*/
	inline const int* triangle_ids() const { return (int*)triangle_ids_.data(); }

	// Set Properties. 
	inline void point(int id, const glm::vec3 &v) { points_[id]=v; }
	inline void normal(int id, const glm::vec3 &n) { normals_[id]=n; }
	inline void uv(int id, float u, float v) { uvs_[id][0]=u; uvs_[id][1]=v; }
	void SetAllNormals(glm::vec3 unified_normal);
	void SetAllUVs(float u, float v);
	void SetColor(float r, float g, float b, float alpha=1.0f);
	

	// File IO
	void WriteObjFormatStream(std::ostream &out, std::string group_name="", int v_offset=0, int vt_offset=0, int vn_offset=0) const;
	void WriteObjFormatStreamV(std::ostream &out) const;	// Vertices.
	void WriteObjFormatStreamVT(std::ostream &out) const;	// UVs.
	void WriteObjFormatStreamVN(std::ostream &out) const;	// Normals.
	void WriteObjFormatStreamG(std::ostream &out, std::string group_name) const;	// Group name.
	void WriteObjFormatStreamF(std::ostream &out, int v_offset=0, int vt_offset=0, int vn_offset=0) const;		// Faces.
	void ReadObjFormatStream(std::istream &in);
	void ReadObjFormatFile(std::string file);


	// operators
	friend Mesh operator*(const glm::mat4 &m, const Mesh& mesh);
	friend Mesh operator+(const Mesh& A_mesh, const Mesh& B_mesh);
	Mesh& operator=(const Mesh& mesh) { Clear(); Assign(mesh); return *this; };
	Mesh& operator+=(const Mesh& mesh) { Merge(mesh); return *this; };


protected:
	void Triangulate();
	void SetPointUvNormalFaceSizes(int num_vertices, int num_uvs, int num_normals, int num_faces);
	void SetFacePointAndNormalIds(int face_id, ...);

	std::vector<glm::vec3> points_;
	std::vector<glm::vec4> colors_;
	std::vector<glm::vec2> uvs_;
	std::vector<glm::vec3> normals_;
	std::vector< glm::ivec3 > triangle_ids_;

	std::vector<int> face_sizes_;
	std::vector< std::vector<int> > face_point_ids_;
	std::vector< std::vector<int> > face_uv_ids_;
	std::vector< std::vector<int> > face_normal_ids_;
};




Mesh operator*(const glm::mat4 &m, const Mesh& mesh);
Mesh operator+(const Mesh& A_mesh, const Mesh& B_mesh);