#include "Integers.h"

#pragma region Int2


Int2::Int2(const int value)
{
	x = value;
	y = value;
}
Int2::Int2(const int x, const int y)
{
	this->x = x;
	this->y = y;

	
}

void Int2::operator+=(const Int2& int1)
{
	x = x + int1.x;
	y = y + int1.y;
}

void Int2::operator-=(const Int2& int1)
{
	x = x - int1.x;
	y = y - int1.y;
}

void Int2::operator*=(const Int2& int1)
{
	x = x * int1.x;
	y = y * int1.y;
}

void Int2::operator/=(const Int2& int1)
{
	x = x / int1.x;
	y = y / int1.y;
}

void Int2::operator%=(const Int2& int1)
{
	x = x % int1.x;
	y = y % int1.y;
}

bool Int2::operator==(const Int2& int1)
{
	return x == int1.x && y == int1.y;
}

bool Int2::operator!=(const Int2& int1)
{
	return x != int1.x || y != int1.y;
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

#pragma endregion

Int3::Int3(const int x, const int y, const int z)
{
	this->x = x;
	this->y = y;
	this->z = z;

}

Int3::Int3(const int value)
{
	x = value;
	y = value;
	z = value;
}

Int4::Int4(const int x, const int y, const int z, const int w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Int4::Int4(const int value)
{
	x = value;
	y = value;
	z = value;
	w = value;
}

UInt2::UInt2(const unsigned int x, const unsigned int y)
{
	this->x = x;
	this->y = y;
}

UInt2::UInt2(const unsigned int value)
{
	x = value;
	y = value;
}

UInt3::UInt3(const unsigned int x, const unsigned int y, const unsigned int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

UInt3::UInt3(const unsigned int value)
{
	x = value;
	y = value;
	z = value;
}

UInt4::UInt4(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

UInt4::UInt4(const unsigned int value)
{
	x = value;
	y = value;
	z = value;
	w = value;
}
