#pragma once

struct Int2
{
	Int2(const int x = 0,const int y = 0);

	int x, y;
};

struct Int3
{
	int x, y,z;
};

struct Int4
{
	int x, y, z, w;
};

struct Uint2
{
	unsigned int x, y;
};

struct Uint3
{
	unsigned int x, y, z;
};

struct Uint4
{
	unsigned int x, y, z, w;
};

Int2 operator+(const Int2& int1, const Int2& int2);
Int2 operator-(const Int2& int1, const Int2& int2);
Int2 operator*(const Int2& int1, const Int2& int2);
Int2 operator/(const Int2& int1, const Int2& int2);
Int2 operator%(const Int2& int1, const Int2& int2);

