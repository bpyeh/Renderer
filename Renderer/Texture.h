#pragma once
#include <vector>
#include <string>

#include "Vector.h"

class Texture
{
public:
	int width;
	int height;
	std::vector<unsigned char> image;

	void loadFromFile(std::string& filename);
};

