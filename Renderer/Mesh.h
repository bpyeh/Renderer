#pragma once

#include <vector>

#include "Vector.h"
#include "dtypes.h"

class Mesh
{
private:
	// Array of 3d coords
	std::vector<mymath::Vec3<double>> vertices;

	// Array of 3-tuples where each elem of each tuple {i, j, k} is an index into vertices
	std::vector<mymath::Vec3<uint32_t>> faces;
	
public:
	Mesh() = delete;
	Mesh(std::vector<mymath::Vec3<double>>& _vertices, std::vector<mymath::Vec3<uint32_t>>& _faces) :
		vertices(std::move(_vertices)), faces(std::move(_faces)) {};
};

class MeshLoader
{
public:
	MeshLoader() = delete;

	// Loads a mesh from obj file
	static std::unique_ptr<Mesh> load(std::string& filename);
};

