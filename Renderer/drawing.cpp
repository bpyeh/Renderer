#include <cmath>

#include "drawing.h"

///////////////////////////////////////////////////
//  "Private" helper methods prototypes
//////////////////////////////////////////////////
void _draw_line(ColorBuffer& color_buffer, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);

// P0 is the top of the triangle, P1/2 is the leftmost/rightmost point of the flat side
void _draw_filled_triangle_flat_bottom(ColorBuffer& color_buffer,
	mymath::Vec2<uint32_t>& P0,
	mymath::Vec2<uint32_t>& P1,
	mymath::Vec2<uint32_t>& P2,
	uint32_t color);
// P0 is the bottom of the triangle, P1/2 is the leftmost/rightmost point of the flat side
void _draw_filled_triangle_flat_top(ColorBuffer& color_buffer,
	mymath::Vec2<uint32_t>& P0,
	mymath::Vec2<uint32_t>& P1,
	mymath::Vec2<uint32_t>& P2,
	uint32_t color);


///////////////////////////////////////////////////////
// Public drawing methods
///////////////////////////////////////////////////////

// Draws a pixel at pos (x,y) of color buffer
void Drawing::draw_pixel(ColorBuffer& color_buffer, int x, int y, uint32_t color)
{
	color_buffer.at(x, y) = color;
}

// Draws a line with endpoints P0 and P1
void Drawing::draw_line(ColorBuffer& color_buffer, mymath::Vec2<uint32_t>& P0, mymath::Vec2<uint32_t>& P1, uint32_t color)
{
	int x1 = P0.x;
	int y1 = P0.y;
	int x2 = P1.x;
	int y2 = P1.y;
	int dx = x2 - x1;
	int dy = y2 - y1;

	// iterate on the longer component of slope
	int side = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	double xStep = ((double)dx) / side;
	double yStep = ((double)dy) / side;
	double curX = x1;
	double curY = y1;
	for (int i = 0; i <= side; i++) {
		draw_pixel(color_buffer, std::round(curX), std::round(curY), color);
		curX += xStep;
		curY += yStep;
	}
}

// Draws a filled triangle with vertices {P0, P1, P2}
// Actually draws up to two triangles, one with a flat top and one with a flat bottom. 
// Points might be passed in any order. 
void Drawing::draw_filled_triangle(
	ColorBuffer& color_buffer, 
	mymath::Vec2<uint32_t>& P0, 
	mymath::Vec2<uint32_t>& P1, 
	mymath::Vec2<uint32_t>& P2,
	uint32_t color)
{
	// 1. sort y vertices so that P0=(x0, y0), P1=(x1, y1), P2=(x2, y2) are s.t. y0 <= y1 <= y2
	if (P2.y < P1.y) {
		std::swap(P2, P1);
	}
	if (P1.y < P0.y) {
		std::swap(P1, P0);
	}
	if (P2.y < P1.y) {
		std::swap(P2, P1);
	}
	/*
	* Possible orientations now: 
	*    P0          P0         P0        P0/P1  -  P1/P0  P0       P0
	*   / \         / \       /    \           \    /      | \     / |
	*  P1  \       /   P1  P1/p2 - P2/p1         P2        |   P1 P1 |
	*    \  \     /  /                                     | /     \ |
	*      \ \   / /                                       |/       \|
	*       P2   P2                                        P2       P2
	*/


	// 2. compute midpoint M of P0 and P2, subdividing the original triangle
	// into two flat top/bottom triangles
	// Midpoint formula:
	// m=(y2-y0)/(x2-x0) (slope)
	// y-y0=m(x-x0) (point slope form)
	// x=(y-y0)/m+x0 (solve for x)
	// x1=(y1-y0)/m+x0 (plug in y1, solve for x1)

	// There is an edge case, where the input triangle ALREADY has a flat
	// top/bottom. 
	// Note: calling order of these are 
	// - The top/bottom
	// - The leftmost point of the flat side
	// - The rightmost point of the flat side
	if (P1.y == P2.y) { 
		if (P1.x < P2.x) {
			_draw_filled_triangle_flat_bottom(color_buffer, P0, P1, P2, color);
		} else {
			_draw_filled_triangle_flat_bottom(color_buffer, P0, P2, P1, color);
		}
		return;
	}
	if (P0.y == P1.y) { 
		if (P0.x < P1.x) {
			_draw_filled_triangle_flat_top(color_buffer, P2, P0, P1, color);
		} else {
			_draw_filled_triangle_flat_top(color_buffer, P2, P1, P0, color);
		}
		return;
	}

	uint32_t Mx, My;
	double m = (double)(P2.y - P0.y) / (P2.x - P0.x);
	int My = P1.y;
	int Mx = (int)((My - P0.y)/m + P0.x);
	mymath::Vec2<uint32_t> midPoint(Mx, My);

	if (midPoint.x < P1.x) {
		_draw_filled_triangle_flat_bottom(color_buffer, P0, midPoint, P1, color);
		_draw_filled_triangle_flat_top(color_buffer, P2, midPoint, P1, color);
	} else {
		_draw_filled_triangle_flat_bottom(color_buffer, P0, P1, midPoint, color);
		_draw_filled_triangle_flat_top(color_buffer, P2, P1, midPoint, color);
	}
}

////////////////////////////////////////////////////////
// Implementation of private helper methods
////////////////////////////////////////////////////////

void _draw_line(ColorBuffer& color_buffer, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	// iterate on the longer component of slope
	int side = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	double xStep = ((double)dx) / side;
	double yStep = ((double)dy) / side;
	double curX = x1;
	double curY = y1;
	for (int i = 0; i <= side; i++) {
		color_buffer.at(std::round(curX), std::round(curY)) = color;
		curX += xStep;
		curY += yStep;
	}
}

// Draws a flat bottom triangle via series of horizontal
// lines.
void _draw_filled_triangle_flat_bottom(ColorBuffer& color_buffer,
	mymath::Vec2<uint32_t>& P0,
	mymath::Vec2<uint32_t>& P1,
	mymath::Vec2<uint32_t>& P2,
	uint32_t color)
{
	double m1 = ((double)(P0.x - P1.x)) / ((double)(P0.y - P1.y)); 
	double m2 = ((double)(P0.x - P2.x)) / ((double)(P0.y - P2.y));
	double x_start = (double)P0.x;
	double x_end = x_start;
	for (int y = P0.y; y <= P2.y; y++) {
		_draw_line(color_buffer, (int)x_start, y, (int)x_end, y, color);
		x_start += m1;
		x_end += m2;
	}
}

void _draw_filled_triangle_flat_top(ColorBuffer& color_buffer,
	mymath::Vec2<uint32_t>& P0,
	mymath::Vec2<uint32_t>& P1,
	mymath::Vec2<uint32_t>& P2,
	uint32_t color)
{
	double m1 = ((double)(P2.x - P0.x)) / ((double)(P2.y - P0.y)); 
	double m2 = ((double)(P2.x - P1.x)) / ((double)(P2.y - P1.y));
	double x_start = (double)P2.x;
	double x_end = (double)P2.x;
	for (int y = P2.y; y >= P0.y; y--) {
		_draw_line(color_buffer, (int)x_start, y, (int)x_end, y, color); 
		x_start -= m1;
		x_end -= m2;
	}
}
