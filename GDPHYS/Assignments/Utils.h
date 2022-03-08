#ifndef UTILS
#define UTILS
#include "MyVector.h"

class Utils
{
public:
	static MyVector offset;
	static MyVector P6ToSFMLPoint(MyVector v);
	static MyVector RotatePoint(MyVector v, float a);
};

#endif // !UTILS
