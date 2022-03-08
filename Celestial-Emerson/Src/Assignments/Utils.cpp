#include "Utils.h"

MyVector Utils::offset = MyVector();

MyVector Utils::P6ToSFMLPoint(MyVector v)
{
	MyVector ret = MyVector(v.x, -v.y);
	return ret + offset;
}