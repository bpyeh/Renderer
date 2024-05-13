#pragma once

#include "Vector.h"

// Library of math utility functions
namespace mymath {
	// Rotates about the X/Y/Z axis respectively
	mymath::Vec3<double> rotate_x(mymath::Vec3<double>, double angle);
	mymath::Vec3<double> rotate_y(mymath::Vec3<double>, double angle);
	mymath::Vec3<double> rotate_z(mymath::Vec3<double>, double angle);
}