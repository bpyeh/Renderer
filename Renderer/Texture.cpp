#include "Texture.h"
#include "third_party/lodepng.h"

void Texture::loadFromFile(std::string& filename)
{
	unsigned w = 0;
	unsigned h = 0;
	unsigned error = lodepng::decode(image, w, h, filename);
	width = w;
	height = h;
}
