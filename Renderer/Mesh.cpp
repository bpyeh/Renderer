#include <string>
#include <fstream>
#include <sstream>
#include <iostream> // debugging only -- take out

#include "Mesh.h"


Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<vertex_t>& v, std::vector<triangle_t>& f)
{
	vertices = v;
	faces = f;
}

void Mesh::load(std::string& filename)
{
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line))
	{
		if (line.length() > 0) {
			if (line.substr(0, 2) == "v ") {
				std::istringstream iss(line);
				std::string v;
				double vx, vy, vz;
				iss >> v >> vx >> vy >> vz;
				vertices.push_back({ vx, vy, vz });
			} else if (line.substr(0, 2) == "f ") {
				std::istringstream iss(line);
				std::string f, p1, p2, p3;
				iss >> f >> p1 >> p2 >> p3;
				int v1Ind, v2Ind, v3Ind;
				int ind = p1.find_first_of('/');
				v1Ind = atoi(p1.substr(0, ind).c_str());
				ind = p2.find_first_of('/');
				v2Ind = atoi(p2.substr(0, ind).c_str());
				ind = p3.find_first_of('/');
				v3Ind = atoi(p3.substr(0, ind).c_str());
				faces.push_back({ v1Ind, v2Ind, v3Ind });
			}
		}
	}
}
