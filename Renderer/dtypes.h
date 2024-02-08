#pragma once

#include <vector>
#include "Vector.h"
// File containing custom data types as they relate to rendering

// vertex described by real world coords
using vertex_t = mymath::Vec3<double>;

// a face is a vector of indices into an array of vertices
using face_t = std::vector<int>;
// a triangle is a face with 3 vertices. TODO: how to assert?
using triangle_t = std::vector<int>;

// a projected triangle is a triangle whose points have been translated into window (2D) coordinates
using projected_triangle_t = struct {
	std::vector<mymath::Vec2<int>> vertices;
	uint32_t color;
	double average_depth;
};