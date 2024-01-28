#pragma once

namespace mymath {
	template<typename T>
	class Vec2 {
	public:
		T x, y;

		Vec2() : x(0), y(0) {};

		Vec2(T inX, T inY)
			: x(inX), y(inY)
		{}
	};

	template<typename T>
	class Vec3 {
	public: 
		T x, y, z;

		Vec3() : x(0), y(0), z(0) {};
		Vec3(T inX, T inY, T inZ)
			: x(inX), y(inY), z(inZ)
		{}

		T mag2() {
			return x * x + y * y + z * z;
		}

		inline Vec3<T> operator+(const Vec3<T>& other) {
			return Vec3<T>(x + other.x, y + other.y, z + other.z);
		}

		inline Vec3<T> operator-(const Vec3<T>& other) {
			return Vec3<T>(x - other.x, y - other.y, z - other.z);
		}

		template<typename S>
		inline Vec3<T> operator*(const S s) {
			return Vec3<T>(x * s, y * s, z * s);
		}

		template<typename S>
		inline Vec3<T> operator/(const S s) {
			return Vec3<T>(x / s, y / s, z / s);
		}

		Vec3<double> normalize() {
			double len = sqrt(mag2());
			return { x / len, y / len, z / len };
		}

		inline T dot(const Vec3<T>& other) {
			return x * other.x + y * other.y + z * other.z;	
		}

		inline Vec3<T> cross(const Vec3<T>& other) {
			return {
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			};
		}
	};

	template<typename T>
	class Vec4 {
	public:
		T x, y, z, w;

		Vec4() : x(0), y(0), z(0), w(0) {};
		Vec4(T inX, T inY, T inZ, T inW)
			: x(inX), y(inY), z(inZ), w(inW)
		{}

		Vec4(Vec3<T>& v) : x(v.x), y(v.y), z(v.z), w(1)
		{}
	};
}

