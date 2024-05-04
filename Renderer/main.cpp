#include <iostream>
#include <vector>
#include "SDL.h"

#include "Window.h"
#include "Vector.h"
#include "data.h"
#include "dtypes.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Texture.h"

const int WINDOW_W = 800;
const int WINDOW_H = 600;
const double ASPECT_RATIO = (double)WINDOW_H / WINDOW_W;
double fov_factor = 3.14159 / 4; // 45 deg
double z_near = 0.1;
double z_far = 100.0;

bool is_running = false;
const int FPS = 30;
const int FRAME_TARGET_TIME = 1000 / FPS;
uint32_t prev_frame_time = 0;
uint32_t cur_frame_time = 0;

void process_input();

Mesh mesh;
Texture texture;
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
mymath::Mat44<double> projection_matrix;
mymath::Vec3<double> camera_pos(0, 0, 0);
mymath::Vec2<int> project_perspective(mymath::Vec3<double>& in, double& z); // this is so ugly, come back and fix

// lighting
mymath::Vec3<double> light_dir(0, 0, 1);
mymath::Vec3<double> negative_light_dir;

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

	{
		Window window(WINDOW_W, WINDOW_H, camera_pos);

		// load mesh
		//std::string filename = "surprised_pikachu.obj";
		std::string filename = "f22.obj";
		//std::string filename = "cube.obj";
		mesh.load(filename);
		triangles.resize(mesh.faces.size());

		// load texture
		filename = "metal_texture.png";
		texture.loadFromFile(filename);

		is_running = window.init();
		
		projection_matrix = mymath::make_perspective(ASPECT_RATIO, fov_factor, z_near, z_far);
		negative_light_dir = light_dir.normalize() * -1.0;

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
    //mesh.rotation.x = 3.14;
	//mesh.rotation.y = -0.01;
	mesh.rotation.y += 0.005;
	//mesh.rotation.z += 0.01;

	mymath::Vec3<double> transformed_pt0;
	mymath::Vec3<double> transformed_pt1;
	mymath::Vec3<double> transformed_pt2;
	num_valid_triangles = 0;
	for (int i = 0; i < mesh.faces.size(); i++) {
		transformed_pt0 = transform(mesh.vertices[mesh.faces[i].vertexInds[0] - 1], mesh.rotation);
		transformed_pt1 = transform(mesh.vertices[mesh.faces[i].vertexInds[1] - 1], mesh.rotation);
		transformed_pt2 = transform(mesh.vertices[mesh.faces[i].vertexInds[2] - 1], mesh.rotation);

		// back face culling
		mymath::Vec3<double> v1 = transformed_pt1 - transformed_pt0;
		mymath::Vec3<double> v2 = transformed_pt2 - transformed_pt0;
		mymath::Vec3<double> face_normal = v1.cross(v2).normalize();
		mymath::Vec3<double> to_camera = camera_pos - transformed_pt0;
		if (to_camera.dot(face_normal) > 0) {
			double z1 = 0, z2 = 0, z3 = 0;
			triangles[num_valid_triangles].vertices[0] = project_perspective(transformed_pt0, z1);
			triangles[num_valid_triangles].vertices[1] = project_perspective(transformed_pt1, z2);
			triangles[num_valid_triangles].vertices[2] = project_perspective(transformed_pt2, z3);
			triangles[num_valid_triangles].average_depth = (z1 + z2 + z3) / 3;
			
			// this is not perspective correct?
			triangles[num_valid_triangles].uvs[0] = mesh.faces[i].uvs[0];
			triangles[num_valid_triangles].uvs[1] = mesh.faces[i].uvs[1];
			triangles[num_valid_triangles].uvs[2] = mesh.faces[i].uvs[2];

			double percentage = negative_light_dir.dot(face_normal); // use magnitude of dot product as intensity of light
			uint32_t color = 0xFFFFFFFF;
			if (percentage < 0) percentage = 0;
			triangles[num_valid_triangles].color = 0xFF000000
				| ((int)(((color & 0x00FF0000) >> 16) * percentage) << 16)
				| ((int)(((color & 0x0000FF00) >> 8) * percentage) << 8)
				| ((int)(((color & 0x000000FF)) * percentage));
				num_valid_triangles++;
		}
	}
}

mymath::Vec2<int> project_perspective(mymath::Vec3<double>& in, double& z) {
	mymath::Vec4<double> homogenized_input(in);
	mymath::Vec4<double> projected = projection_matrix * homogenized_input;
	if (projected.w != 0) {
		projected.x /= projected.w;
		projected.y /= projected.w;
		projected.z /= projected.w;
	}

	// hack here for z sorting
	z = projected.w;

	return {
		(int)(projected.x * (WINDOW_W / 2) + (WINDOW_W / 2)),
		// negate y here to account for model coordinates vs screen coordinates
		(int)(projected.y * (-(WINDOW_H / 2)) + (WINDOW_H / 2))
	};
}
