#pragma once

#include <vector>

#include "Vector.h"
#include "dtypes.h"

class Mesh
{
private:
public:

	// Array of 3d coords
	std::vector<vertex_t> vertices;
	// Array of 3-tuples where each elem of each tuple {i, j, k} is an index into vertices
	std::vector<triangle_t> faces;

	mymath::Vec3<double> rotation;
	Mesh();
	Mesh(std::vector<vertex_t>& v, std::vector<triangle_t>& f);

	void load(std::string& filename);
};

