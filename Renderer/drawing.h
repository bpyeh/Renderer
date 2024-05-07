#pragma once
#include <vector>

#include "ColorBuffer.h"
#include "Vector.h"

// Library of drawing primitives
namespace Drawing {
	// Draws a pixel at pos (x,y) of color buffer
	void draw_pixel(ColorBuffer& color_buffer, int x, int y, uint32_t color); 

	// Draws a line with endpoints P0 and P1
	void draw_line(ColorBuffer& color_buffer, mymath::Vec2<uint32_t>& P0, mymath::Vec2<uint32_t>& P1, uint32_t color);

	// Draws a filled triangle with vertices {P0, P1, P2}
	void draw_filled_triangle(
		ColorBuffer& color_buffer,
		mymath::Vec2<uint32_t>& P0, 
		mymath::Vec2<uint32_t>& P1, 
		mymath::Vec2<uint32_t>& P2, 
		uint32_t color);

}