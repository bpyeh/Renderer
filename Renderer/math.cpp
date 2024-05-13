#include <cmath>

#include "math.h"

mymath::Vec3<double> mymath::rotate_x(mymath::Vec3<double> in, double angle) {
	return {
		in.x,
		in.y * cos(angle) - in.z * sin(angle),
		in.y * sin(angle) + in.z * cos(angle)
	};
}

mymath::Vec3<double> mymath::rotate_y(mymath::Vec3<double> in, double angle)
{
	return {
		in.x * cos(angle) - in.z * sin(angle),
		in.y,
		in.x * sin(angle) + in.z * cos(angle)
	};
}

mymath::Vec3<double> mymath::rotate_z(mymath::Vec3<double> in, double angle) {
	return {
		in.x * cos(angle) - in.y * sin(angle),
		in.x * sin(angle) + in.y * cos(angle),
		in.z
	};
}
