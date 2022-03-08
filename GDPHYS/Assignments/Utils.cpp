#include "Utils.h"

MyVector Utils::offset = MyVector();

MyVector Utils::P6ToSFMLPoint(MyVector v)
{
	MyVector ret = MyVector(v.x, -v.y);
	return ret + offset;
}

MyVector Utils::RotatePoint(MyVector v, float a)
{
	MyVector ret = MyVector();
	float rad = a;

	ret.x = v.x * cos(rad) + v.y * sin(rad);
	ret.y = -v.x * sin(rad) + v.y * cos(rad);

	return ret;
}