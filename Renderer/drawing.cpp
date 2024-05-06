#include "drawing.h"

// draws a pixel at pos (x,y) of color buffer
void Drawing::draw_pixel(ColorBuffer& color_buffer, int x, int y, uint32_t color)
{
	color_buffer.at(x, y) = color;
}


