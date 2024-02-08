#include <iostream>
#include <algorithm>

#include "Window.h"
#include "dtypes.h"

Window::Window(int width, int height, mymath::Vec3<double> camera_pos)
	: W(width), H(height), camera(camera_pos)
{
	color_buffer.resize(width * height);
}

Window::~Window()
{
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}

	if (window) {
		SDL_DestroyWindow(window);
	}
}

bool Window::init()
{
	// TODO: how to assert SDL_INIT has been called?

	window = SDL_CreateWindow(
		nullptr,
		SDL_WINDOWPOS_CENTERED, // pos x
		SDL_WINDOWPOS_CENTERED, // pox y
		W,         // w
		H,        // h
		SDL_WINDOW_BORDERLESS
	);

	if (nullptr == window) {
		std::cerr << "Error creating SDL Window" << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

	if (nullptr == renderer) {
		std::cerr << "Error creating SDL Renderer" << std::endl;
		return false;
	}

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, // a flag for repeated access
		W,
		H
	);

	return true;
}

void Window::render(std::vector<projected_triangle_t>& pts, int N)
{
	// some custom draw function here that fills the color buffer
	draw_triangles(pts, N);

	SDL_UpdateTexture(         // copies 
		color_buffer_texture,  // this texture buffer 
		NULL,                  // ...
		color_buffer.data(),   // with data from this pointer
		(int)(sizeof(uint32_t) * W)
	);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);

	// clear color buffer
	fill(color_buffer.begin(), color_buffer.end(), 0xFF000000);

	SDL_RenderPresent(renderer);
}

void Window::draw_triangles(std::vector<projected_triangle_t>& pts, int N)
{
	// painter's algorithm
	// sort by z first before drawing
	sort(pts.begin(), pts.end(),
		[](const projected_triangle_t& t1, const projected_triangle_t& t2) {
			return t1.average_depth > t2.average_depth;
		});

	for (int i = 0; i < N; i++) {
		//draw_rect(pts[i][0].x, pts[i][0].y, 3, 3, 0xFFFFFF00);
		//draw_rect(pts[i][1].x, pts[i][1].y, 3, 3, 0xFFFFFF00);
		//draw_rect(pts[i][2].x, pts[i][2].y, 3, 3, 0xFFFFFF00);
		draw_line(pts[i].vertices[0].x, pts[i].vertices[0].y, pts[i].vertices[1].x, pts[i].vertices[1].y, pts[i].color);
		draw_line(pts[i].vertices[1].x, pts[i].vertices[1].y, pts[i].vertices[2].x, pts[i].vertices[2].y, pts[i].color);
		draw_line(pts[i].vertices[2].x, pts[i].vertices[2].y, pts[i].vertices[0].x, pts[i].vertices[0].y, pts[i].color);
		
		draw_filled_triangle(
			pts[i].vertices[0].x, pts[i].vertices[0].y,
			pts[i].vertices[1].x, pts[i].vertices[1].y,
			pts[i].vertices[2].x, pts[i].vertices[2].y,
			pts[i].color);
	}
}

// (x0, y0) is the "top" of the triangle
void Window::draw_filled_triangle_flat_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	double m1 = ((double)(x0 - x1)) / ((double)(y0 - y1)); // note inverse, b/c of horizontal scanline e.g. we are looping in y 
	double m2 = ((double)(x0 - x2)) / ((double)(y0 - y2));
	//std::cout << m1 << " " << m2 << std::endl;
	double x_start = (double)x0;
	double x_end = (double)x0;
	for (int y = y0; y <= y2; y++) {
		//std::cout << x_start << " " << y << " " << x_end << " " << y << std::endl;
		draw_line((int)x_start, y, (int)x_end, y, color); // todo: this can be optimized to only deal in integers
		x_start += m1;
		x_end += m2;
	}
}

// (x2, y2) is the "bottom" of the triangle
void Window::draw_filled_triangle_flat_top(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	//std::cout << "debug" << std::endl;
	//std::cout << "(" << x0 << ',' << y0 << "), (" << x1 << ',' << y1 << "), (" << x2 << ',' << y2 << ")" << std::endl;
	double m1 = ((double)(x2 - x0)) / ((double)(y2 - y0)); // note inverse, b/c of horizontal scanline e.g. we are looping in y 
	double m2 = ((double)(x2 - x1)) / ((double)(y2 - y1));
	//std::cout << m1 << " " << m2 << std::endl;
	double x_start = (double)x2;
	double x_end = (double)x2;
	for (int y = y2; y >= y0; y--) {
		//std::cout << "(" << x_start << ", " << y << "), (" << x_end << ", " << y << ")" << std::endl;
		draw_line((int)x_start, y, (int)x_end, y, color); // todo: this can be optimized to only deal in integers
		x_start -= m1;
		x_end -= m2;
	}
}

void Window::draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	// 1. sort y vertices so that (x0, y0), (x1, y1), (x2, y2) are s.t. y0 <= y1 <= y2
	if (y2 < y1) {
		std::swap(x2, x1); std::swap(y2, y1);
	}
	if (y1 < y0) {
		std::swap(x1, x0); std::swap(y1, y0);
	}
	if (y2 < y1) {
		std::swap(x2, x1); std::swap(y2, y1);
	}



	if (/* y0 <= */ y1 == y2) { // don't render lower triangle
		draw_filled_triangle_flat_bottom(x0, y0, x1, y1, x2, y2, color);
		return;
	}
	if (y0 == y1 /* <= y2 */) { // don't render upper triangle
		draw_filled_triangle_flat_top(x0, y0, x1, y1, x2, y2, color);
		return;
	}

	// 2. compute midpoint
	// (Mx-x0)/(x2-x0) = (My-y0)/(y2-y0)
	// (Mx-x0)(y2-y0) = (My-y0)(x2-x0)
	int My = y1;
	int Mx = (int)(((double)((My - y0) * (x2 - x0))) / (y2 - y0) + x0);
	draw_filled_triangle_flat_bottom(x0, y0, Mx, My, x1, y1, color);
	draw_filled_triangle_flat_top(x1, y1, Mx, My, x2, y2, color);

	//std::cout << "(" << x0 << "," << y0 << ")" << std::endl;
	//std::cout << "(" << x1 << "," << y1 << ")" << std::endl;
	//std::cout << "(" << x2 << "," << y2 << ")" << std::endl;
	//std::cout << "(" << Mx << "," << My << ")" << std::endl;
}

void Window::draw_pixel(int x, int y, uint32_t color)
{
	if (x >= 0 && y >= 0 && x < W && y < H)
		color_buffer[y * W + x] = color;
}

void Window::draw_line(int x1, int y1, int x2, int y2, uint32_t color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int side = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	
	double xStep = ((double)dx) / side;
	double yStep = ((double)dy) / side;
	double curX = x1;
	double curY = y1;
	for (int i = 0; i <= side; i++) {
		draw_pixel(std::round(curX), std::round(curY), color);
		curX += xStep;
		curY += yStep;
	}
}

// todo: rewrite in terms of x and y only
void Window::draw_rect(int x, int y, int width, int height, uint32_t color)
{
	int r_limit = std::min(y + height, H);
	int c_limit = std::min(x + width, W);
	x = x >= 0 ? x : 0;
	y = y >= 0 ? y : 0;
	for (int r = y; r < r_limit; r++) {
		for (int c = x; c < c_limit; c++) {
			draw_pixel(c, r, color); 
		}
	}
}
