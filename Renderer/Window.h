#pragma once

/*
 Class containing rendering contexts such as SDL_Window and SDL_Renderer.
 Also includes drawing functions.

*/

#include <vector>

#include "SDL.h"
#include "Vector.h"
#include "dtypes.h"

class Window
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* color_buffer_texture = nullptr;

	mymath::Vec3<double> camera;

	std::vector<uint32_t> color_buffer;
	// TODO: define color buffer-specific sizes, and window-specific sizes
	const int W;
	const int H;

	void draw_filled_triangle_flat_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
	void draw_filled_triangle_flat_top(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);


public:
	Window(int width, int height, mymath::Vec3<double> camera_pos);
	~Window();

	bool init();
	void render(std::vector<projected_triangle_t>& vec, int N);

	// custom draw functions
	void draw_triangles(std::vector<projected_triangle_t>& vec, int N);
	void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
	void draw_pixel(int x, int y, uint32_t color);
	void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
	void draw_rect(int x, int y, int width, int height, uint32_t color);
};

