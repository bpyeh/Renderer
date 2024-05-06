#include "ColorBuffer.h"

uint32_t& ColorBuffer::operator[](int i)
{
	return buf[i];
}

uint32_t& ColorBuffer::at(int x, int y)
{
	if (x >= 0 && y >= 0 && x < W && y < H) {
		return buf[y * W + x];
	} else {
		throw "Indexing color buffer out of bounds";
	}
}
