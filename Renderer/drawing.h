#pragma once
#include <vector>

#include "ColorBuffer.h"

namespace Drawing {
	// draws a pixel at pos (x,y) of color buffer
	void draw_pixel(ColorBuffer& color_buffer, int x, int y, uint32_t color); 
}