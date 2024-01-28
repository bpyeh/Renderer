#pragma once
#include <vector>

#include "Vector.h"
#include "dtypes.h"

// file containing hardcoded values for shapes, meshes, etc

std::vector<vertex_t> CUBE_MESH_VERTICES = {
	{-1, -1, -1}, // 1
	{-1, 1, -1},  // 2
	{1, 1, -1},   // 3
	{1, -1, -1},  // 4
	{1, 1, 1},    // 5
	{1, -1, 1},   // 6
	{-1, 1, 1},   // 7
	{-1, -1, 1},  // 8
};

std::vector<triangle_t> CUBE_MESH_FACES = {
	{1, 2, 3},
	{1, 3, 4}, // front
	{4, 3, 5}, 
	{4, 5, 6}, // right side
	{6, 5, 7},
	{6, 7, 8}, // back
	{8, 7, 2},
	{8, 2, 1}, // left side
	{6, 8, 1},
	{6, 1, 4}, // bottom
	{2, 7, 5}, 
	{2, 5, 3}  // top
};
