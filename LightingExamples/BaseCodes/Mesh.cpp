
#include "Mesh.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <map>

static glm::vec3 y_axis(0.f, 1.f, 0.f);
static glm::vec3 x_axis(1.f, 0.f, 0.f);
static glm::vec3 z_axis(0.f, 0.f, 1.f);

Mesh::Mesh()
{
}

Mesh::Mesh(const Mesh& mesh)
{
	Assign(mesh);
}


Mesh::~Mesh()
{
	Clear();
}

void
Mesh::Clear()
{
	points_.clear();

	normals_.clear();

	face_sizes_.clear();

	triangle_ids_.clear();

	colors_.clear();

	face_point_ids_.clear();

	face_uv_ids_.clear();

	face_normal_ids_.clear();

	
}


void
Mesh::SetPointUvNormalFaceSizes(int num_vertices, int num_uvs, int num_normals, int num_faces)
{

	points_.resize(num_vertices);
	uvs_.resize(num_uvs);
	normals_.resize(num_normals);
	face_sizes_.resize(num_faces);

	if ( face_sizes_.size() > 0 )
	{
		face_point_ids_.resize(num_faces);
		if ( num_uvs!=0 )face_uv_ids_.resize(num_faces);
		if ( num_normals!=0 ) face_normal_ids_.resize(num_faces);
	}
}


void
Mesh::SetFacePointAndNormalIds(int face_id, ...)
{
	va_list vl;

	va_start(vl,face_id);

	for ( int i=0; i<face_sizes_[face_id]; i++)
	{
		int v_id = va_arg(vl, int);
		int n_id = va_arg(vl, int);
		face_point_ids_[face_id][i] = v_id;
		face_normal_ids_[face_id][i] = n_id;
	}

	va_end(vl);

}






/// Transform
void
Mesh::TranslateVertices(glm::vec3 t)
{
	for ( unsigned int i=0; i< points_.size(); i++ )
	{
		point(i, point(i)+t);
	}
}

void
Mesh::RotateVertices(glm::quat q, glm::vec3 center)
{
	glm::vec3 v;
	for ( unsigned int i = 0; i < points_.size(); i++ )
	{
		v = rotate(q, point(i)-center) + center;
		point(i, v);
	}

	for ( unsigned int i=0; i<normals_.size(); i++ )
	{
		v = rotate(q, normal(i));
		normal(i, v);
	}
}

void
Mesh::RotateVertices(float angle, glm::vec3 axis)
{

	glm::quat q = glm::angleAxis(angle, axis);
	RotateVertices(q);
}

void
Mesh::ScaleUniformlyVertices(float s, glm::vec3 center)
{
	glm::vec3 v;
	for ( unsigned int i=0; i<points_.size(); i++ )
	{
		v = s*(point(i)-center) + center;
		point(i, v);
	}
}

void
Mesh::ScaleVertices(glm::vec3 s, glm::vec3 center)
{
	glm::vec3 v;
	for ( unsigned int i=0; i<points_.size(); i++ )
	{
		v = (point(i)-center);
		v[0] *= s[0];
		v[1] *= s[1];
		v[2] *= s[2];
		v += center;
		point(i, v);
	}

	for ( unsigned int i=0; i<normals_.size(); i++ )
	{
		v = normal(i);
		v[0] *= s[0];
		v[1] *= s[1];
		v[2] *= s[2];
		v = glm::normalize(v);
		normal(i, v);
	}
}

void
Mesh::TransformVertices(const glm::mat4 &m)
{
	glm::vec3 v;
	for (unsigned int i = 0; i < points_.size(); i++)
	{
		v = glm::vec3(m*glm::vec4(point(i), 1.f));
		point(i, v);
	}

	glm::mat4 m_it = glm::transpose(glm::inverse(m));
	for (unsigned int i = 0; i < normals_.size(); i++)
	{
		v = glm::vec3(m_it*glm::vec4(normal(i), 0.f));
		v = glm::normalize(v);
		normal(i, v);
	}
}







/// Create Premitives

void
Mesh::CreateBox(float width, float height, float depth)
{
	Clear();

	/// Allocation
	SetPointUvNormalFaceSizes(24, 0, 6, 6);

	for ( unsigned int i=0; i<face_sizes_.size(); i++ )
	{
		face_sizes_[i] = 4;
		face_point_ids_[i].resize( face_sizes_[i] );
		face_normal_ids_[i].resize( face_sizes_[i] );
	}

	/// Set Data
	float half_w = 0.5f * width;
	float half_h = 0.5f * height;
	float half_d = 0.5f * depth;

	glm::vec3 corners[8];
	corners[0] = glm::vec3(-half_w,  half_h, +half_d);
	corners[1] = glm::vec3(+half_w,  half_h, +half_d);
	corners[2] = glm::vec3(+half_w,  half_h, -half_d);
	corners[3] = glm::vec3(-half_w,  half_h, -half_d);

	corners[4] = glm::vec3(-half_w, -half_h, +half_d);
	corners[5] = glm::vec3(+half_w, -half_h, +half_d);
	corners[6] = glm::vec3(+half_w, -half_h, -half_d);
	corners[7] = glm::vec3(-half_w, -half_h, -half_d);

	point( 0, corners[ 0] );
	point( 1, corners[ 1] );
	point( 2, corners[ 2] );
	point( 3, corners[ 3] );

	point( 4, corners[ 7] );
	point( 5, corners[ 6] );
	point( 6, corners[ 5] );
	point( 7, corners[ 4] );

	point( 8, corners[ 2] );
	point( 9, corners[ 1] );
	point(10, corners[ 5] );
	point(11, corners[ 6] );

	point(12, corners[ 0] );
	point(13, corners[ 3] );
	point(14, corners[ 7] );
	point(15, corners[ 4] );

	point(16, corners[ 0] );
	point(17, corners[ 4] );
	point(18, corners[ 5] );
	point(19, corners[ 1] );

	point(20, corners[ 2] );
	point(21, corners[ 6] );
	point(22, corners[ 7] );
	point(23, corners[ 3] );

	normal(0,  x_axis);
	normal(1, -x_axis);
	normal(2,  y_axis);
	normal(3, -y_axis);
	normal(4,  z_axis);
	normal(5, -z_axis);

	SetFacePointAndNormalIds(0, 0, 2
								, 1, 2 
								, 2, 2 
								, 3, 2);
	
	SetFacePointAndNormalIds(1, 4, 3
								, 5, 3
								, 6, 3 
								, 7, 3);

	SetFacePointAndNormalIds(2, 8, 0
								, 9, 0
								, 10, 0 
								, 11, 0);

	SetFacePointAndNormalIds(3, 12, 1
								, 13, 1
								, 14, 1 
								, 15, 1);

	SetFacePointAndNormalIds(4, 16, 4
								, 17, 4
								, 18, 4 
								, 19, 4);
	
	SetFacePointAndNormalIds(5, 20, 5
								, 21, 5
								, 22, 5 
								, 23, 5);

}


void
Mesh::CreateCylinder(glm::vec3 bottom_center, glm::vec3 top_center, float radius, int slices, int stacks)
{

	glm::vec3 v = top_center-bottom_center;
	float h = glm::length(v);
	glm::vec3 n = normalize(v);
	
	glm::vec3 rotation_axis;
	float rotation_angle;

	if ( glm::length(n-y_axis) <= 0.0001 )
	{
		rotation_axis = glm::vec3(0, 1, 0);
		rotation_angle = 0;
	}
	else
	{
		rotation_axis = glm::cross(y_axis, n);
		rotation_angle = atan2( glm::length(rotation_axis), glm::dot(y_axis,n) );
		rotation_axis = glm::normalize(rotation_axis);
	}
	
	CreateCylinder(h, radius, slices, stacks);
	TranslateVertices(glm::vec3(0, 0.5*h, 0));
	RotateVertices(glm::angleAxis(rotation_angle, rotation_axis));
	TranslateVertices(bottom_center);
}

void
Mesh::CreateCylinder(float height, float radius, int slices, int stacks)
{
	Clear();

	glm::vec3 top    = glm::vec3(0, height/2, 0);
	glm::vec3 bottom = -1.f * top;

	std::vector< glm::vec3> round_points;
	for ( int i=0; i<slices; i++ )
	{
		glm::vec3 tmp_p = glm::rotate( glm::angleAxis(i*(2*glm::pi<float>())/slices, glm::vec3(0, 1, 0))
								, glm::vec3(0, 0, radius) );
		round_points.push_back( tmp_p );
	}

	SetPointUvNormalFaceSizes( 2 + 2*slices + (stacks+1)*slices,  0, round_points.size()+2, (stacks+2)*slices );

	
	// set vertices
	{
		int v_count = 0;
		// top center, index 0;
		point(v_count, top);			
		v_count++;
		// bottom center, index 1;
		point(v_count, bottom);		
		v_count++;

		// top and bottom circles.
		for ( int i=0; i<2; i++ )
		{
			glm::vec3 y;
			if ( i==0 ) y = top;
			else y = bottom;

			for ( unsigned int j=0; j<round_points.size(); j++ )
			{
				point(v_count, y + round_points[j]);
				v_count++;
			}
		}

		// Side vertices from top to bottom. From vertex index 2.
		for ( int i=0; i<=stacks; i++ )
		{
			glm::vec3 y = top - glm::vec3(0, (height/stacks)*i, 0);
			if ( i==stacks ) y = bottom;

			for ( unsigned int j=0; j<round_points.size(); j++ )
			{
				point(v_count, y + round_points[j]);
				v_count++;
			}
		}

		//gtest		EXPECT_EQ( num_vertices_, v_count );
	}

	// set normals
	{
		int n_count = 0;
		// top, index 0
		normal(n_count, y_axis);			
		n_count++;

		// bottom, index 1
		normal(n_count, -1.f*y_axis);		
		n_count++;

		// Side normals, from index 2 to 2+slices-1.
		for ( int j=0; j<slices; j++ )
		{
			//gtest EXPECT_LE( n_count, num_normals_ );
			normal(n_count, glm::normalize(round_points[j]));
			n_count++;
		}
	}

	// set faces
	{
		// Allocate for face indices.
		for ( int i=0; i<(int)face_sizes_.size(); i++ )
		{
			// top and bottom circle-faces.
			if ( i<2*slices )
			{
				face_sizes_[i] = 3;
			}

			// Side Walls.
			else
			{
				face_sizes_[i] = 4;
			}

			face_point_ids_[i].resize( face_sizes_[i] );
			face_normal_ids_[i].resize( face_sizes_[i] );
		}

		int f_count = 0;
		// triangles for top circular face.
		for ( int i=0; i<slices; i++ )
		{
			if ( i==slices-1 )
			{
				SetFacePointAndNormalIds(f_count, 0, 0, 
									i+2, 0, 
									2, 0);
				f_count++;
			}
			else
			{
				SetFacePointAndNormalIds(f_count, 0, 0, 
									i+2, 0, 
									i+3, 0);
				f_count++;
			}
		}

		// triangles for bottom circular face.
		for ( int i=0; i<slices; i++ )
		{
			if ( i==slices-1 )
			{
				SetFacePointAndNormalIds(f_count, 1, 1, 
									2+2*slices-i-1, 1, 
									2+2*slices-1, 1);
				f_count++;
			}
			else
			{
				SetFacePointAndNormalIds(f_count, 1, 1, 
									2+2*slices-i-1, 1, 
									2+2*slices-i-2, 1);
				f_count++;
			}
		}


		// Side wall quads.
		for ( int i=0; i<stacks; i++ )
		{
			for ( int j=0; j<slices; j++ )
			{
				if ( j==slices-1 )
				{
					int left_top_vertex_id     = 2+2*slices+(slices*i)+j;
					int left_botoom_vertex_id  = 2+2*slices+(slices*(i+1))+j;
					int right_botoom_vertex_id = 2+2*slices+(slices*(i+1));
					int right_top_vertex_id    = 2+2*slices+(slices*i);
					int left_normal_id  = 2+j;
					int right_normal_id = 2;
					SetFacePointAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
				else
				{
					int left_top_vertex_id     = 2+2*slices+(slices*i)+j;
					int left_botoom_vertex_id  = 2+2*slices+(slices*(i+1))+j;
					int right_botoom_vertex_id = 2+2*slices+(slices*(i+1))+j+1;
					int right_top_vertex_id    = 2+2*slices+(slices*i)+j+1;
					int left_normal_id  = 2+j;
					int right_normal_id = 2+j+1;
					SetFacePointAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
			}
		}
	}
}

void
Mesh::CreateOpenedCylinder(float height, float radius, int slices, int stacks)
{
	Clear();

	glm::vec3 top    = glm::vec3(0, height/2, 0);
	glm::vec3 bottom = -1.f * top;

	std::vector< glm::vec3> round_points;
	for ( int i=0; i<slices; i++ )
	{
		glm::vec3 tmp_p = glm::rotate( glm::angleAxis(i*(2*glm::pi<float>())/slices, glm::vec3(0, 1, 0))
								, glm::vec3(0, 0, radius) );
		round_points.push_back( tmp_p );
	}

	SetPointUvNormalFaceSizes( (stacks+1)*slices,  0, round_points.size(), stacks*slices );

	
	// set vertices
	{
		int v_count = 0;

		// Side vertices from top to bottom. From vertex index 2.
		for ( int i=0; i<=stacks; i++ )
		{
			glm::vec3 y = top - glm::vec3(0, (height/stacks)*i, 0);
			if ( i==stacks ) y = bottom;

			for ( unsigned int j=0; j<round_points.size(); j++ )
			{
				point(v_count, y + round_points[j]);
				v_count++;
			}
		}

		//gtest EXPECT_EQ( num_vertices_, v_count );
	}

	// set normals
	{
		int n_count = 0;

		// Side normals, from index 2 to 2+slices-1.
		for ( int j=0; j<slices; j++ )
		{
			//gtest EXPECT_LE( n_count, num_normals_ );
			normal(n_count, normalize(round_points[j]));
			n_count++;
		}
	}

	// set faces
	{
		// Allocate for face indices.
		for ( unsigned int i=0; i<face_sizes_.size(); i++ )
		{
			face_sizes_[i] = 4;
			face_point_ids_[i].resize( face_sizes_[i] );
			face_normal_ids_[i].resize( face_sizes_[i] );
		}

		int f_count = 0;

		// Side wall quads.
		for ( int i=0; i<stacks; i++ )
		{
			for ( int j=0; j<slices; j++ )
			{
				if ( j==slices-1 )
				{
					int left_top_vertex_id     = (slices*i)+j;
					int left_botoom_vertex_id  = (slices*(i+1))+j;
					int right_botoom_vertex_id = (slices*(i+1));
					int right_top_vertex_id    = (slices*i);
					int left_normal_id  = j;
					int right_normal_id = 0;
					SetFacePointAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
				else
				{
					int left_top_vertex_id     = (slices*i)+j;
					int left_botoom_vertex_id  = (slices*(i+1))+j;
					int right_botoom_vertex_id = (slices*(i+1))+j+1;
					int right_top_vertex_id    = (slices*i)+j+1;
					int left_normal_id  = j;
					int right_normal_id = j+1;
					SetFacePointAndNormalIds(f_count, 
											left_top_vertex_id,     left_normal_id, 
											left_botoom_vertex_id,  left_normal_id, 
											right_botoom_vertex_id, right_normal_id, 
											right_top_vertex_id,    right_normal_id);
					//gtest EXPECT_LE( f_count, num_faces_ );
					f_count++;
				}
			}
		}
	}
}

void
Mesh::CreateCapsule(float cylinder_height, float radius)
{
	Clear();

	CreateOpenedCylinder(cylinder_height, radius);
	UpdateNormalVectorsBasedOnFace();

	// Top Hemisphere.
	Mesh top_hemi;
	top_hemi.CreateHemisphere(radius);
	top_hemi.TranslateVertices(glm::vec3(0, cylinder_height/2, 0));
	top_hemi.UpdateNormalVectorsBasedOnVertex();

	// Bottom Hemisphere.
	Mesh bottom_hemi;
	bottom_hemi.CreateHemisphere(radius);
	bottom_hemi.RotateVertices(glm::angleAxis(glm::pi<float>(), glm::vec3(0, 0, 1)));
	bottom_hemi.TranslateVertices(glm::vec3(0, -1*cylinder_height/2, 0));
	bottom_hemi.UpdateNormalVectorsBasedOnVertex();

	// Merge
	Merge(top_hemi);
	Merge(bottom_hemi);
}

void
Mesh::WriteObjFormatStreamV(std::ostream &out) const
{
	// vertices
	for (unsigned int i = 0; i < points_.size(); i++)
	{
		out << "v " << points_[i][0] 
		    << " "  << points_[i][1]
		    << " "  << points_[i][2]
		    << std::endl;
	}
}

void
Mesh::WriteObjFormatStreamVT(std::ostream &out) const
{
	// uvs
	for (unsigned int i = 0; i < uvs_.size(); i++)
	{
		out << "vt " << uvs_[i][0] 
		    << " "  << uvs_[i][1]
		    << std::endl;
	}
}

void
Mesh::WriteObjFormatStreamVN(std::ostream &out) const
{
	// normals
	for (unsigned int i = 0; i < normals_.size(); i++)
	{
		out << "vn " << normals_[i][0] 
		    << " "  << normals_[i][1]
		    << " "  << normals_[i][2]
		    << std::endl;
	}
}

void
Mesh::WriteObjFormatStreamF(std::ostream &out, int v_offset, int vt_offset, int vn_offset) const
{
	
	// faces
	if ( !face_point_ids_.empty() && !face_uv_ids_.empty() && !face_normal_ids_.empty() )
	{
		// vertex/uv/normal
		for ( unsigned int i=0; i<face_sizes_.size(); i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_point_ids_[i][j]+1+v_offset
					<< "/" << face_uv_ids_[i][j]+1+vt_offset
					<< "/" << face_normal_ids_[i][j]+1+vn_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
	else if ( !face_point_ids_.empty() && !face_normal_ids_.empty() )
	{
		// vertex//normal
		for ( unsigned int i=0; i<face_sizes_.size(); i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_point_ids_[i][j]+1+v_offset
					<< "//" << face_normal_ids_[i][j]+1+vn_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
	else if ( !face_point_ids_.empty() && !face_uv_ids_.empty() )
	{
		// vertex/uv
		for ( unsigned int i=0; i<face_sizes_.size(); i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_point_ids_[i][j]+1+v_offset
					<< "/" << face_uv_ids_[i][j]+1+vt_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
	else if ( !face_point_ids_.empty() )
	{
		// vertex
		for ( unsigned int i=0; i<face_sizes_.size(); i++ )
		{
			out << "f ";
			for ( int j=0; j<face_sizes_[i]; j++ )
			{
				out << face_point_ids_[i][j]+1+v_offset
					<< " ";
			}		
			out << std::endl;
		}
	}
}

void
Mesh::WriteObjFormatStreamG(std::ostream &out, std::string group_name) const
{
	out << "g " << group_name << std::endl;
}

void
Mesh::WriteObjFormatStream(std::ostream &out, std::string group_name, int v_offset, int vt_offset, int vn_offset) const
{
	WriteObjFormatStreamV(out);
	WriteObjFormatStreamVT(out);
	WriteObjFormatStreamVN(out);
	WriteObjFormatStreamG(out, group_name);
	WriteObjFormatStreamF(out, v_offset, vt_offset, vn_offset);
}

void
Mesh::ReadObjFormatStream(std::istream &in)
{
	Clear();

	// Count numburs of each element.
	int num_v = 0;
	int num_vt = 0;
	int num_vn = 0;
	int num_f = 0;

	std::string line;
	while ( !in.eof() )
	{
		std::getline(in, line);
		if ( in.fail() ) break;

		if ( line.compare(0, 2, "v ") == 0 ) num_v++;
		else if ( line.compare(0, 2, "f ") == 0 ) num_f++;
		else if ( line.compare(0, 3, "vt ") == 0 ) num_vt++;
		else if ( line.compare(0, 3, "vn ") == 0 ) num_vn++;
	}

	// Allocate
	SetPointUvNormalFaceSizes(num_v, num_vt, num_vn, num_f);

	in.clear();
	in.seekg(0);
	std::stringstream sstr_line;
	std::string word;
	int count_v=0;
	int count_vt=0;
	int count_vn=0;
	int count_f=0;
	while ( !in.eof() )
	{
		std::getline(in, line);
		if ( in.fail() ) break;

		sstr_line.clear();
		sstr_line.str(line);

		sstr_line >> word;

		if ( word.compare("v") ==  0 )
		{
			float x, y, z;
			sstr_line >> x >> y >> z;
			points_[count_v][0] = x;
			points_[count_v][1] = y;
			points_[count_v][2] = z;
			count_v++;
		}
		else if ( word.compare("vt") ==  0 )
		{
			float u, v;
			sstr_line >> u >> v;
			uvs_[count_vt][0] = u;
			uvs_[count_vt][1] = v;
			count_vt++;
		}
		else if ( word.compare("vn") ==  0 )
		{
			float x, y, z;
			sstr_line >> x >> y >> z;
			normals_[count_vn][0] = x;
			normals_[count_vn][1] = y;
			normals_[count_vn][2] = z;
			count_vn++;
		}
		else if ( word.compare("f") ==  0 )
		{
			std::vector<std::string> points;

			while ( !sstr_line.eof() )
			{
				sstr_line >> word;
				if ( word.empty() ) break;
				if ( sstr_line.fail() ) break;
				points.push_back(word);
			}

			int num_points = (int)points.size();
			face_sizes_[count_f] = num_points;

			if ( num_points > 0 )
			{
				if ( num_v > 0 && num_vt > 0 && num_vn > 0 )
				{
					face_point_ids_[count_f] .resize(num_points);
					face_uv_ids_[count_f]     .resize(num_points);
					face_normal_ids_[count_f] .resize(num_points);

					for ( int i=0; i<num_points; i++ )
					{
						std::string tmp;
						// vertex
						tmp.assign(points[i].begin(), points[i].begin()+points[i].find("/"));
						face_point_ids_[count_f][i] = std::stoi(tmp)-1;
						// uv
						tmp.assign(points[i].begin()+points[i].find("/")+1, points[i].begin()+points[i].rfind("/"));
						face_uv_ids_[count_f][i] = std::stoi(tmp)-1;
						// normal
						tmp.assign(points[i].begin()+points[i].rfind("/")+1, points[i].end());
						face_normal_ids_[count_f][i] = std::stoi(tmp)-1;
					}
				}
				else if ( num_v > 0 && num_vt > 0 && num_vn == 0 )
				{
					face_point_ids_[count_f] .resize(num_points);
					face_uv_ids_[count_f]     .resize(num_points);

					for ( int i=0; i<num_points; i++ )
					{
						std::string tmp;
						// vertex
						tmp.assign(points[i].begin(), points[i].begin()+points[i].find("/"));
						face_point_ids_[count_f][i] = std::stoi(tmp)-1;
						// uv
						tmp.assign(points[i].begin()+points[i].find("/")+1, points[i].end());
						face_uv_ids_[count_f][i] = std::stoi(tmp)-1;
					}
				}
				else if ( num_v > 0 && num_vt == 0 && num_vn > 0 )
				{
					face_point_ids_[count_f] .resize(num_points);
					face_normal_ids_[count_f] .resize(num_points);

					for ( int i=0; i<num_points; i++ )
					{
						std::string tmp;
						// vertex
						tmp.assign(points[i].begin(), points[i].begin()+points[i].find("/"));
						face_point_ids_[count_f][i] = std::stoi(tmp)-1;
						// normal
						tmp.assign(points[i].begin()+points[i].rfind("/")+1, points[i].end());
						face_normal_ids_[count_f][i] = std::stoi(tmp)-1;
					}
				}
				else if ( num_v > 0 && num_vt == 0 && num_vn == 0 )
				{
					face_point_ids_[count_f] .resize(num_points);

					for ( int i=0; i<num_points; i++ )
					{
						face_point_ids_[count_f][i] = std::stoi(points[i])-1;
					}
				}
			}

			count_f++;
		}
	}
}

void
Mesh::ReadObjFormatFile(std::string file)
{
	std::ifstream fin(file.c_str());
	ReadObjFormatStream(fin);
	fin.close();
}


glm::vec3
Mesh::CalculFaceNormal(int face_id) const
{
	if ( face_id >= (int)face_sizes_.size() ) return glm::vec3(0, 0, 0);
	if ( face_point_ids_.empty() ) return glm::vec3(0, 0, 0);
	if ( face_sizes_[face_id] < 2 ) return glm::vec3(0, 0, 0);
	

	glm::vec3 vertex0 = point(face_point_ids_[face_id][0]);
	glm::vec3 vertex1 = point(face_point_ids_[face_id][1]);
	glm::vec3 vertex2 = point(face_point_ids_[face_id][2]);
	
	glm::vec3 side0 = vertex1 - vertex0; 
	glm::vec3 side1 = vertex2 - vertex1;

	return normalize(glm::cross(side0,side1));
}

glm::vec3
Mesh::CalculVertexNormal(int vertex_id) const
{
	if ( vertex_id >= (int)points_.size() ) return glm::vec3(0, 0, 0);
	if ( face_sizes_.empty() ) return glm::vec3(0, 0, 0);
	if ( face_point_ids_.empty() ) return glm::vec3(0, 0, 0);


	// "normal_v" will be the average vector of all normal vectors of faces, which are having the vertex of "vertex_id" as a point.
	glm::vec3 normal_v(0, 0, 0);
	int num_of_faces_including_the_vertex = 0;
	for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	{
		// Check all points of this face.
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			// If this point is same to the "vertex_id",
			// then calculate the normal of the face of "face_id"
			// and add it to "normal_v".
			if ( face_point_ids_[face_id][point_id] == vertex_id )
			{
				num_of_faces_including_the_vertex++;
				normal_v += CalculFaceNormal(face_id);
				break;
			}
		}
	}

	if ( num_of_faces_including_the_vertex == 0 ) return glm::vec3(0, 0, 0);
	normal_v = normal_v / (float)num_of_faces_including_the_vertex;

	return glm::normalize(normal_v);
}

void
Mesh::UpdateNormalVectorsBasedOnFace()
{
	if ( points_.empty() || face_sizes_.empty() || face_point_ids_.empty() ) return;

	/// Clear Normals, if they exist.
	normals_.clear();

	/// Allocate Space for Normal Vectors.
	// The number of normal vertors is same to the number of faces.
	normals_.resize(face_sizes_.size());

	/// Set Normals per Face.
	for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	{
		normal(face_id, CalculFaceNormal(face_id));
	}

	/// Allocate space for face_normal_ids_, if it didn't.
	{
		face_normal_ids_.resize(face_sizes_.size());
		for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
		{
			face_normal_ids_[face_id].resize(face_sizes_[face_id]);
		}
	}
	


	/// Set Normal Ids.
	// The normal id for every point in a face will be assigned to the normal id of the face normal.
	for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_normal_ids_[face_id][point_id] = face_id;
		}
	}

}

void
Mesh::UpdateNormalVectorsBasedOnVertex()
{
	if ( points_.empty() || face_sizes_.empty() || face_point_ids_.empty() ) return;

	/// Clear Normals, if they exist.
	normals_.clear();

	/// Allocate Space for Normal Vectors.
	// The number of normal vertors is same to the number of vertices.
	normals_.resize(points_.size());


	/// Set Normals per Vertex.
	for ( unsigned int vertex_id=0; vertex_id<points_.size(); vertex_id++ )
	{
		normal(vertex_id, CalculVertexNormal(vertex_id));
	}

	face_normal_ids_.clear();
	face_normal_ids_.assign(face_point_ids_.begin(), face_point_ids_.end());
	///// Allocate space for face_normal_ids_, if it didn't.
	//if ( face_normal_ids_.empty() )
	//{
	//	face_normal_ids_.resize(face_sizes_.size());
	//	for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	//	{
	//		face_normal_ids_[face_id].resize(face_sizes_[face_id]);
	//	}
	//}
	//


	///// Set Normal Ids.
	//// The normal id for a point in a face will be assigned to the normal id of the corresponding vertex normal.
	//for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	//{
	//	for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
	//	{
	//		face_normal_ids_[face_id][point_id] = face_vertex_ids_[face_id][point_id];
	//	}
	//}
}

void
Mesh::SetAllNormals(glm::vec3 unified_normal)
{
	if ( points_.empty() || face_sizes_.empty() || face_point_ids_.empty() ) return;

	/// Clear Normals, if they exist.
	normals_.clear();

	/// Allocate Space for Normal Vectors.
	// The number of normal vertors is 1.
	normals_.resize(1);

	normal(0, unified_normal);

	/// Allocate space for face_normal_ids_, if it didn't.
	if ( face_normal_ids_.empty() )
	{
		face_normal_ids_.resize(face_sizes_.size());
		for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
		{
			face_normal_ids_[face_id].resize(face_sizes_[face_id]);
		}
	}

	/// Set All Normal Ids as 0.
	for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_normal_ids_[face_id][point_id] = 0;
		}
	}
}

void
Mesh::SetAllUVs(float u, float v)
{
	if ( points_.empty() || face_sizes_.empty() || face_point_ids_.empty() ) return;

	/// Clear UV, if they exist.
	uvs_.clear();


	/// Allocate Space for UVs.
	// The number of UV is 1.
	uvs_.resize(1);

	uv(0, u, v);

	/// Allocate space for face_uv_ids_, if it didn't.
	if ( face_uv_ids_.empty() )
	{
		face_uv_ids_.resize(face_sizes_.size());
		for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
		{
			face_uv_ids_[face_id].resize(face_sizes_[face_id]);
		}
	}

	/// Set All UV Ids as 0.
	for ( unsigned int face_id=0; face_id<face_sizes_.size(); face_id++ )
	{
		for ( int point_id=0; point_id<face_sizes_[face_id]; point_id++ )
		{
			face_uv_ids_[face_id][point_id] = 0;
		}
	}
}


void
Mesh::SetColor(float r, float g, float b, float alpha)
{
	if ( points_.empty() ) return;


	if ( colors_.size() != points_.size() )
	{
		colors_.resize(points_.size());
	}

	for ( unsigned int i=0; i<points_.size(); i++ )
	{
		colors_[i][0] = r;
		colors_[i][1] = g;
		colors_[i][2] = b;
		colors_[i][3] = alpha;
	}
}

void
Mesh::Assign(const Mesh &in_mesh)
{
	if ( in_mesh.num_points() == 0 || in_mesh.num_faces() == 0 ) return;
	const Mesh &src = in_mesh;

	Clear();

	points_.assign(src.points_.begin(), src.points_.end());
	normals_. assign(src.normals_.begin() , src.normals_.end());
	uvs_.     assign(src.uvs_.begin()     , src.uvs_.end());
	colors_.  assign(src.colors_.begin()  , src.colors_.end());

	face_sizes_.       assign(src.face_sizes_.begin()     , src.face_sizes_.end());
	face_point_ids_.  assign(src.face_point_ids_.begin(), src.face_point_ids_.end());
	face_normal_ids_.  assign(src.face_normal_ids_.begin(), src.face_normal_ids_.end());
	face_uv_ids_.      assign(src.face_uv_ids_.begin()    , src.face_uv_ids_.end());

	
	if ( src.num_triangles() > 0 ) 
	{
		triangle_ids_.assign(src.triangle_ids_.begin(), src.triangle_ids_.end());
	}
}

void
Mesh::Merge(const Mesh &in_mesh)
{
	// If this mesh is empty, then assign the incoming.
	if ( this->num_points() == 0 || this->num_faces() == 0 )
	{
		Assign(in_mesh);
		return;
	}

	// if the input is empty, then return.
	if ( in_mesh.num_points() == 0 || in_mesh.num_faces() == 0 ) return;


	// A is source mesh, a copy of 'this'.
	// B is new mesh which will be merged with A.
	Mesh &A = *this;//; A.Assign(*this);
	const Mesh &B = in_mesh; // B.Assign(in_mesh);

	/// To make the merge process easier, make both memory states as Same.
	// 1. If B has normals, but A doesn't
	if ( B.num_normals() > 0 && A.num_normals() == 0 )
	{
		A.SetAllNormals(x_axis);
	}
	// 2. If B has uvs, but A doesn't
	if ( B.num_uvs() > 0 && A.num_uvs() == 0 )
	{
		A.SetAllUVs(0, 0);
	}
	


	int num_vertices_of_A = A.num_points();
	int num_normals_of_A  = A.num_normals();
	int num_uvs_of_A      = A.num_uvs();
	int num_faces_of_A    = A.num_faces();
	

	Mesh &C = *this; // target

	C.points_.insert(C.points_.end(), B.points_.begin(), B.points_.end());
	C.normals_ .insert(C.normals_.end() , B.normals_.begin() , B.normals_.end());
	C.uvs_     .insert(C.uvs_.end()     , B.uvs_.begin()     , B.uvs_.end());
	C.colors_  .insert(C.colors_.end()  , B.colors_.begin()  , B.colors_.end());

	C.face_sizes_     .insert(C.face_sizes_.end()     , B.face_sizes_.begin()     , B.face_sizes_.end());
	C.face_point_ids_.insert(C.face_point_ids_.end(), B.face_point_ids_.begin(), B.face_point_ids_.end());
	C.face_uv_ids_    .insert(C.face_uv_ids_.end()    , B.face_uv_ids_.begin()    , B.face_uv_ids_.end());
	C.face_normal_ids_.insert(C.face_normal_ids_.end(), B.face_normal_ids_.begin(), B.face_normal_ids_.end());


	// Fix indices of B in C
	for ( unsigned int i=num_faces_of_A; i<C.face_sizes_.size(); i++ )
	{
		for ( auto &d : C.face_point_ids_[i] )
		{
			d += num_vertices_of_A;
		}
	}

	if ( !C.face_normal_ids_.empty() )
	{
		for ( unsigned int i=num_faces_of_A; i<C.face_sizes_.size(); i++ )
		{
			//// If A had normals, but B doesn't
			while ( C.face_normal_ids_.size() <= i )
			{
				C.face_normal_ids_.push_back( std::vector<int>( C.face_sizes_[i], -1 ) );
			}

			for ( auto &d : C.face_normal_ids_[i] )
			{
				d += num_normals_of_A;
			}
		}
	}

	if ( !C.face_uv_ids_.empty() )
	{
		for ( unsigned int i=num_faces_of_A; i<C.face_sizes_.size(); i++ )
		{
			//// If A had normals, but B doesn't
			while ( C.face_uv_ids_.size() <= i )
			{
				C.face_uv_ids_.push_back( std::vector<int>( C.face_sizes_[i], -1 ) );
			}

			for ( auto &d : C.face_uv_ids_[i] )
			{
				d += num_uvs_of_A;
			}
		}
	}

	   

	if ( !A.triangle_ids_.empty() && !B.triangle_ids_.empty() )
	{
		int num_tri_of_A = A.num_triangles();

		C.triangle_ids_.insert(C.triangle_ids_.end()   , B.triangle_ids_.begin()   , B.triangle_ids_.end());

		for ( unsigned int i=num_tri_of_A; i<C.triangle_ids_.size(); i++ )
		{
			C.triangle_ids_[i][0] += num_vertices_of_A;
			C.triangle_ids_[i][1] += num_vertices_of_A;
			C.triangle_ids_[i][2] += num_vertices_of_A;
		}
	}
	else
	{
		C.triangle_ids_.clear();
	}




}


void
Mesh::Triangulate()
{
	if ( num_faces() ==  0 ) return;

	
	{
		for ( int i=0; i<num_faces(); i++ )
		{
			if ( face_sizes_[i] > 3 )
			{
				for ( int j=3; j<face_sizes_[i]; j++ )
				{
					face_sizes_.push_back(3);

					if ( !face_point_ids_.empty() )
					{
						face_point_ids_.push_back( std::vector<int>() );
						face_point_ids_.back().push_back( face_point_ids_[i][0  ] );
						face_point_ids_.back().push_back( face_point_ids_[i][j-1] );
						face_point_ids_.back().push_back( face_point_ids_[i][j  ] );
					}


					if ( !face_normal_ids_.empty() )
					{
						face_normal_ids_.push_back( std::vector<int>() );
						face_normal_ids_.back().push_back( face_normal_ids_[i][0  ] );
						face_normal_ids_.back().push_back( face_normal_ids_[i][j-1] );
						face_normal_ids_.back().push_back( face_normal_ids_[i][j  ] );
					}

					if ( !face_uv_ids_.empty() )
					{
						face_uv_ids_.push_back( std::vector<int>() );
						face_uv_ids_.back().push_back( face_uv_ids_[i][0  ] );
						face_uv_ids_.back().push_back( face_uv_ids_[i][j-1] );
						face_uv_ids_.back().push_back( face_uv_ids_[i][j  ] );
					}
				}

				if ( !face_point_ids_.empty() ) face_point_ids_[i].resize(3);
				if ( !face_normal_ids_.empty() ) face_normal_ids_[i].resize(3);
				if ( !face_uv_ids_.empty() ) face_uv_ids_[i].resize(3);
				face_sizes_[i] = 3;
			}
		}


	
	}

	/*triangle_ids_.resize(num_faces());
	for ( int i=0; i<num_faces(); i++ )
	{
		triangle_ids_[i][0] = face_point_ids_[i][0];
		triangle_ids_[i][1] = face_point_ids_[i][1];
		triangle_ids_[i][2] = face_point_ids_[i][2];
	}*/

	
}

static bool operator<(const glm::ivec3 &a, const glm::ivec3 &b )
{
	if ( a[0] < b[0] ) return true;
	if ( a[0] > b[0] ) return false;
	if ( a[1] < b[1] ) return true;
	if ( a[1] > b[1] ) return false;
	if ( a[2] < b[2] ) return true;
	
	return false;
}

void
Mesh::TriangulateAndUniformizeBuffers()
{
	Triangulate();

	if ( colors_.size() != points_.size() ) colors_.resize( points_.size(), glm::vec4(1.f, 1.f, 1.f, 1.f) );

	if ( face_normal_ids_.empty() ) SetAllNormals(glm::vec3(1.f, 0.f, 0.f));
	if ( face_uv_ids_.empty() ) SetAllUVs(0.f, 0.f);

	
	std::map< std::tuple<int, int ,int>, int> face_vertex_lists;	// the key (tuple) is (face_vertex_id, face_normal_id, face_uv_id) of a vertex. the data (int) is unique index.
	int face_vertex_lists_count = 0;
	for ( unsigned int f=0; f<face_sizes_.size(); f++ )   
	{
		for ( int i=0; i<3; i++ )
		{
			std::tuple<int, int ,int> fv = std::make_tuple(face_point_ids_[f][i], face_normal_ids_[f][i], face_uv_ids_[f][i]);

			if ( face_vertex_lists.find(fv) == face_vertex_lists.end() )
			{
				face_vertex_lists[fv] = face_vertex_lists_count++;
			}
		}
	}


	std::vector<glm::vec3> t_points(face_vertex_lists.size());
	std::vector<glm::vec3> t_normals(face_vertex_lists.size());
	std::vector<glm::vec2> t_uvs(face_vertex_lists.size());


	std::vector< std::vector<int> > t_face_ids(face_sizes_.size(), std::vector<int>(3));

	
	for ( unsigned int f=0; f<face_sizes_.size(); f++ )   
	{
		for ( int i=0; i<3; i++ )
		{
			std::tuple<int, int ,int> fv = std::make_tuple(face_point_ids_[f][i], face_normal_ids_[f][i], face_uv_ids_[f][i]);

			int ii = face_vertex_lists[fv];

			t_points [ii] = points_ [ std::get<0>(fv) ];
			t_normals[ii] = normals_[ std::get<1>(fv) ];
			t_uvs    [ii] = uvs_    [ std::get<2>(fv) ];

			t_face_ids[f][i] = ii;
		}
	}

	points_.clear();
	normals_.clear();
	uvs_.clear();

	face_point_ids_.clear();
	face_normal_ids_.clear();
	face_uv_ids_.clear();

	points_.assign(t_points.begin(), t_points.end());
	normals_.assign(t_normals.begin(), t_normals.end());
	uvs_.assign(t_uvs.begin(), t_uvs.end());

	face_point_ids_.assign (t_face_ids.begin(), t_face_ids.end() );;
	face_normal_ids_.assign (t_face_ids.begin(), t_face_ids.end() );
	face_uv_ids_.assign (t_face_ids.begin(), t_face_ids.end() );


	triangle_ids_.resize(face_sizes_.size());
	for ( unsigned int i=0; i<face_sizes_.size(); i++ )
	{
		triangle_ids_[i][0] = face_point_ids_[i][0];
		triangle_ids_[i][1] = face_point_ids_[i][1];
		triangle_ids_[i][2] = face_point_ids_[i][2];
	}

}


///////////////////////////////
// Operators
Mesh operator*(const glm::mat4 &m, const Mesh& mesh)
{
	Mesh result;
	result = mesh;
	result.TransformVertices(m);

	return result;
}


Mesh operator+(const Mesh& A_mesh, const Mesh& B_mesh)
{
	Mesh result;
	result = A_mesh;
	result.Merge(B_mesh);

	return result;
}







