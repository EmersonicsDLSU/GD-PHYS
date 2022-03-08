#include "Utils.h"

MyVector Utils::offset = MyVector();

MyVector Utils::P6ToSFMLPoint(MyVector v)
{
	MyVector ret = v;
	return ret + offset;
}