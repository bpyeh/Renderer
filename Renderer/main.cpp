#include <iostream>
#include <vector>
#include "SDL.h"

#include "Window.h"
#include "Vector.h"
#include "data.h"
#include "dtypes.h"
#include "Mesh.h"
#include "Matrix.h"

const int WINDOW_W = 800;
const int WINDOW_H = 600;
const double ASPECT_RATIO = (double)WINDOW_H / WINDOW_W;

bool is_running = false;
const int FPS = 30;
const int FRAME_TARGET_TIME = 1000 / FPS;
uint32_t prev_frame_time = 0;
uint32_t cur_frame_time = 0;

void process_input();

Mesh mesh;
std::vector<projected_triangle_t> triangles;
int num_valid_triangles = 0;

// simple orthographic projection onto XY plane
mymath::Vec2<int> project_iso(mymath::Vec3<double> in) {
	int fov_factor = 128; // tune
	return { 
		(int)(in.x * fov_factor) + (WINDOW_W/2), 
		(int)(in.y * fov_factor) + (WINDOW_H/2)
	};
}

// perspective projection
mymath::Vec3<double> camera_pos(0, 0, 0);
mymath::Vec2<int> project_perspective(mymath::Vec3<double> in);

mymath::Vec3<double> transform(mymath::Vec3<double> in, mymath::Vec3<double> rot);
mymath::Vec3<double> rotate_x(mymath::Vec3<double>, double angle);
mymath::Vec3<double> rotate_y(mymath::Vec3<double>, double angle);
mymath::Vec3<double> rotate_z(mymath::Vec3<double>, double angle);
void update();

int main(int argc, char* args[]) {

	if (0 != SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Error initializing SDL" << std::endl;
		return false;
	}

	mymath::Mat44<int> sM = mymath::make_scale(2, 3, 4);
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			std::cout << "[" << sM[r][c] << "]";
		}
		std::cout << std::endl;
	}

	{
		Window window(WINDOW_W, WINDOW_H, camera_pos);
		//mesh = Mesh(CUBE_MESH_VERTICES, CUBE_MESH_FACES);
		//std::string filename = "surprised_pikachu.obj";
		//std::string filename = "f22.obj";
		std::string filename = "cube.obj";
		mesh.load(filename);
		triangles.resize(mesh.faces.size(), { { 0, 0 }, { 0, 0 }, { 0, 0 } });
		is_running = window.init();

		while (is_running) {
			process_input();
			update();
			window.render(triangles, num_valid_triangles);
		}
	}

	SDL_Quit();
	return 0;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT: // triggered when 'x' on window is clicked
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (SDLK_ESCAPE == event.key.keysym.sym) {
			is_running = false;
		}
	default:
		break;
	}
}

mymath::Vec3<double> rotate_x(mymath::Vec3<double> in, double angle) {
	return {
		in.x,
		in.y * cos(angle) - in.z * sin(angle),
		in.y * sin(angle) + in.z * cos(angle)
	};
}

mymath::Vec3<double> rotate_y(mymath::Vec3<double> in, double angle)
{
	return {
		in.x * cos(angle) - in.z * sin(angle),
		in.y,
		in.x * sin(angle) + in.z * cos(angle)
	};
}
mymath::Vec3<double> rotate_z(mymath::Vec3<double> in, double angle) {
	return {
		in.x * cos(angle) - in.y * sin(angle),
		in.x * sin(angle) + in.y * cos(angle),
		in.z
	};
}

mymath::Vec3<double> transform(mymath::Vec3<double> in, mymath::Vec3<double> rot) {
	mymath::Vec3<double> out = rotate_z(rotate_y(rotate_x(in, rot.x), rot.y), rot.z);
	
	//out.z -= camera_pos.z;
	out.z += 5;
	return out;
}


void update() {
	// TODO: learn to use std::chrono instead
	cur_frame_time = SDL_GetTicks();
	int dt = (int)cur_frame_time - (int)prev_frame_time;
	int time_to_wait = FRAME_TARGET_TIME - dt;
	if (time_to_wait >= 0 && time_to_wait < FRAME_TARGET_TIME) {
		dt += time_to_wait;
		SDL_Delay(time_to_wait);
	}
	std::cout << dt << ", FPS: " << (1000.0/dt) << std::endl;

	prev_frame_time = cur_frame_time;

	//mesh.rotation.x += 0.01;
    mesh.rotation.x = 3.14;
	//mesh.rotation.y = -0.01;
	mesh.rotation.y += 0.01;
	//mesh.rotation.z += 0.01;

	//triangles[0] = {
	//	{100, 100},
	//	{200, 50},
	//	{300, 300}
	//};
	//num_valid_triangles = 1;

	mymath::Vec3<double> transformed_pt0;
	mymath::Vec3<double> transformed_pt1;
	mymath::Vec3<double> transformed_pt2;
	num_valid_triangles = 0;
	for (int i = 0; i < mesh.faces.size(); i++) {
		transformed_pt0 = transform(mesh.vertices[mesh.faces[i][0] - 1], mesh.rotation);
		transformed_pt1 = transform(mesh.vertices[mesh.faces[i][1] - 1], mesh.rotation);
		transformed_pt2 = transform(mesh.vertices[mesh.faces[i][2] - 1], mesh.rotation);

		// back face culling
		mymath::Vec3<double> v1 = transformed_pt1 - transformed_pt0;
		mymath::Vec3<double> v2 = transformed_pt2 - transformed_pt0;
		mymath::Vec3<double> face_normal = v1.cross(v2);
		mymath::Vec3<double> to_camera = camera_pos - transformed_pt0;
		if (to_camera.dot(face_normal) > 0) {
			triangles[num_valid_triangles][0] = project_perspective(transformed_pt0);
			triangles[num_valid_triangles][1] = project_perspective(transformed_pt1);
			triangles[num_valid_triangles][2] = project_perspective(transformed_pt2);
			num_valid_triangles++;
		}
	}
}

mymath::Vec2<int> project_perspective(mymath::Mat44<double>& P, mymath::Vec4<double>& in) {
	mymath::Vec4<double> projected = P * in;
	if (projected.w != 0) {
		projected.x /= projected.w;
		projected.y /= projected.w;
		projected.z /= projected.w;
	}
}

mymath::Vec2<int> project_perspective(mymath::Vec3<double> in) {
	int fov_factor = 640; // 1 / tan(theta/2)
	return {
		(int)((in.x * fov_factor) / in.z) + (WINDOW_W / 2),
		(int)((in.y * fov_factor) / in.z) + (WINDOW_H / 2)
	};
}
