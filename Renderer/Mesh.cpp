#include <string>
#include <fstream>
#include <sstream>
#include <iostream> 

#include "Mesh.h"

const std::string VERTEX_PREFIX = { "v " };
const std::string FACE_PREFIX = { "f " };

struct FaceToken {
	int ind1;
	int unused1, unused2; // TODO: find out what these are for

	friend std::istream& operator>>(std::istream& in, FaceToken& f) {
		char slash;
		in >> f.ind1 >> slash >> f.unused1 >> slash >> f.unused2;
		return in;
	}
};

std::unique_ptr<Mesh> MeshLoader::load(std::string& filename)
{
	std::ifstream file(filename);
	std::string line;

	double vx, vy, vz;
	FaceToken t1, t2, t3;
	std::string unused;
	std::vector<mymath::Vec3<double>> vertices;
	std::vector<mymath::Vec3<uint32_t>> faces;

	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		if (line.compare(0, VERTEX_PREFIX.length(), VERTEX_PREFIX) == 0) {
			iss >> unused >> vx >> vy >> vz;
			vertices.emplace_back(vx, vy, vz);
		} else if (line.compare(0, FACE_PREFIX.length(), FACE_PREFIX) == 0) {
			iss >> unused >> t1 >> t2 >> t3;
			faces.emplace_back(t1.ind1, t2.ind1, t3.ind1);
		}
	}
	return std::make_unique<Mesh>(vertices, faces);
}
