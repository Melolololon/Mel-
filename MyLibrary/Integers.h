#pragma once

struct Int2
{
	Int2(const int value = 0);
	Int2(const int x,const int y);
	

	int x, y;

	void operator+=(const Int2& int1);
	void operator-=(const Int2& int1);
	void operator*=(const Int2& int1);
	void operator/=(const Int2& int1);
	void operator%=(const Int2& int1);
	bool operator==(const Int2& int1);
	bool operator!=(const Int2& int1);
};

Int2 operator+(const Int2& int1, const Int2& int2);
Int2 operator-(const Int2& int1, const Int2& int2);
Int2 operator*(const Int2& int1, const Int2& int2);
Int2 operator/(const Int2& int1, const Int2& int2);
Int2 operator%(const Int2& int1, const Int2& int2);




struct Int3
{
	Int3(const int value = 0);
	Int3(const int x, const int y, const int z);
	

	int x, y,z;
};

struct Int4
{
	Int4(const int value = 0);
	Int4(const int x, const int y , const int z , const int w );
	

	int x, y, z, w;
};

struct UInt2
{
	UInt2(const unsigned int value = 0);
	UInt2(const unsigned int x, const unsigned int y);

	unsigned int x, y;
};

struct UInt3
{
	UInt3(const unsigned int value = 0);
	UInt3(const unsigned int x , const unsigned int y, const unsigned int z);
	
	unsigned int x, y, z;
};

struct UInt4
{
	UInt4(const unsigned int value = 0);
	UInt4(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w);
	

	unsigned int x, y, z, w;
};


