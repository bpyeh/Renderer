#pragma once
#include <cmath>
#include <vector>

#include "Vector.h"

namespace mymath {
	template<typename T>
	class Mat22 {
	public:
		T a, b;
		T c, d;
		Mat22(T _a, T _b, T _c, T _d) 
			: a(_a), b(_b), c(_c), d(_d)
		{}
	};

	// helper function to create a 2D rotation (CCW) matrix 
	Mat22<double> rot(double rad) {
		return Mat22<double>(
			cos(rad), -sin(rad),
			sin(rad), cos(rad)
		);
	}

	template<typename T>
	class Mat44 {
	public:
		std::vector<std::vector<T>> matrix{ {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };

		Mat44() {}

		std::vector<T>& operator[](int i) {
			return matrix[i];
		}

		Vec4<T> operator*(const Vec4<T>& vec) {
			return Vec4<T>(
				matrix[0][0] * vec.x + matrix[0][1] * vec.y + matrix[0][2] * vec.z,
				matrix[1][0] * vec.x + matrix[1][1] * vec.y + matrix[1][2] * vec.z,
				matrix[2][0] * vec.x + matrix[2][1] * vec.y + matrix[2][2] * vec.z,
				matrix[3][0] * vec.x + matrix[3][1] * vec.y + matrix[3][2] * vec.z
			);
		}
	};

	template<typename T> 
	static Mat44<T> eye() {
		Mat44<T> out;
		out[0][0] = 1;
		out[1][1] = 1;
		out[2][2] = 1;
		out[3][3] = 1;
		return out;
	}

	template<typename T>
	static Mat44<T> make_scale(T sx, T sy, T sz) {
		Mat44<T> out = eye<T>();
		out[0][0] = sx;
		out[1][1] = sy;
		out[2][2] = sz;
		return out;
	}

	template<typename T>
	static Mat44<T> make_translate(T tx, T ty, T tz) {
		Mat44<T> out = eye<T>();
		out[0][3] = tx;
		out[1][3] = ty;
		out[2][3] = tz;
		return out;
	}

	template<typename T>
	static Mat44<T> make_rotateZ(double rad) {
		Mat44<T> out = eye<T>();
		out[0][0] = cos(rad); out[0][1] = -sin(rad);
		out[1][0] = sin(rad); out[1][1] = cos(rad);
		return out;
	}

	template<typename T>
	static Mat44<T> make_rotateX(double rad) {
		Mat44<T> out = eye<T>();
		out[1][1] = cos(rad); out[1][2] = -sin(rad);
		out[2][1] = sin(rad); out[2][2] = cos(rad);
		return out;
	}

	template<typename T>
	static Mat44<T> make_rotateY(double rad) {
		Mat44<T> out = eye<T>();
		out[0][0] = cos(rad); out[0][2] = sin(rad);
		out[2][0] = -sin(rad); out[2][2] = cos(rad);
		return out;
	}

	static Mat44<double> make_perspective(double aspect_ratio, double fov, double znear, double zfar) {
		Mat44<double> out = eye<double>();
		out[0][0] = aspect_ratio / tan(fov / 2);
		out[1][1] = 1 / tan(fov / 2);
		out[2][2] = zfar / (zfar - znear);
		out[2][3] = -zfar * znear / (zfar - znear);
		out[3][2] = 1;
		out[3][3] = 0;
		return out;
	}
}