#include "Vector.h"

template<typename T>
mymath::Vec2<T>::Vec2(T inX, T inY)
	: x(inX), y(inY)
{

}

template mymath::Vec2<int>::Vec2(int inX, int inY);