#pragma once
#include <vector>

class ColorBuffer
{
private:
	uint32_t W, H;

	std::vector<uint32_t> buf;
public:

	uint32_t getW() const { return W; }
	uint32_t getH() const { return H; }

	uint32_t& operator[] (int i);

	uint32_t& at(int x, int y);
};

