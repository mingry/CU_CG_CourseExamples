#pragma once

#include "Mesh.h"

void InitBasicShapeObjs();
void DrawSphere();
void DrawCylinder();
void DrawCapsule();
void DrawHemisphere();
void DrawBox();
void DeleteBasicShapeObjs();



extern Mesh g_sphere_mesh;
extern Mesh g_hemisphere_mesh;
extern Mesh g_box_mesh;
extern Mesh g_cylinder_mesh;
extern Mesh g_capsule_mesh;