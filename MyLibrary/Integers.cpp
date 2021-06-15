#include "Integers.h"


Int2::Int2(const int x, const int y)
{
	this->x = x;
	this->y = y;
}

Int2 operator+(const Int2& int1, const Int2& int2)
{
	return Int2(int1.x + int2.x, int1.y + int2.y);
}

Int2 operator-(const Int2& int1, const Int2& int2)
{
	return Int2(int1.x - int2.x, int1.y - int2.y);
}

Int2 operator*(const Int2& int1, const Int2& int2)
{
	return Int2(int1.x * int2.x, int1.y * int2.y);
}

Int2 operator/(const Int2& int1, const Int2& int2)
{
	return Int2(int1.x / int2.x, int1.y / int2.y);
}

Int2 operator%(const Int2& int1, const Int2& int2)
{
	return Int2(int1.x % int2.x, int1.y % int2.y);
}

