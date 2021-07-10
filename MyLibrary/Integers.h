#pragma once

//Int4にnt2、Int3を引数としたコンストラクタ用意しておけば、
//Int2とInt3はInt4を引数としたコンストラクタ用意すればもう一つ用意しなくてもいい


struct Int2;
struct Int3;
struct Int4;
struct UInt2;
struct UInt3;
struct UInt4;


struct Int2
{
	Int2(const int value = 0);
	Int2(const int x,const int y);
	Int2(const Int3& int3);
	Int2(const Int4& int4);

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
	Int3(const Int2& int2);
	Int3(const Int4& int4);

	int x, y,z;

	void operator+=(const Int3& int1);
	void operator-=(const Int3& int1);
	void operator*=(const Int3& int1);
	void operator/=(const Int3& int1);
	void operator%=(const Int3& int1);
	bool operator==(const Int3& int1);
	bool operator!=(const Int3& int1);
};
Int3 operator+(const Int3& int1, const Int3& int2);
Int3 operator-(const Int3& int1, const Int3& int2);
Int3 operator*(const Int3& int1, const Int3& int2);
Int3 operator/(const Int3& int1, const Int3& int2);
Int3 operator%(const Int3& int1, const Int3& int2);

struct Int4
{
	Int4(const int value = 0);
	Int4(const int x, const int y , const int z , const int w );
	Int4(const Int2& int2);
	Int4(const Int3& int3);

	int x, y, z, w;
	void operator+=(const Int4& int1);
	void operator-=(const Int4& int1);
	void operator*=(const Int4& int1);
	void operator/=(const Int4& int1);
	void operator%=(const Int4& int1);
	bool operator==(const Int4& int1);
	bool operator!=(const Int4& int1);
};
Int4 operator+(const Int4& int1, const Int4& int2);
Int4 operator-(const Int4& int1, const Int4& int2);
Int4 operator*(const Int4& int1, const Int4& int2);
Int4 operator/(const Int4& int1, const Int4& int2);
Int4 operator%(const Int4& int1, const Int4& int2);

struct UInt2
{
	UInt2(const unsigned int value = 0);
	UInt2(const unsigned int x, const unsigned int y);
	UInt2(const UInt3& uInt3);
	UInt2(const UInt4& uInt4);

	unsigned int x, y;
	
	void operator+=(const UInt2& int1);
	void operator-=(const UInt2& int1);
	void operator*=(const UInt2& int1);
	void operator/=(const UInt2& int1);
	void operator%=(const UInt2& int1);
	bool operator==(const UInt2& int1);
	bool operator!=(const UInt2& int1);
};
UInt2 operator+(const UInt2& uInt1, const UInt2& uInt2);
UInt2 operator-(const UInt2& uInt1, const UInt2& uInt2);
UInt2 operator*(const UInt2& uInt1, const UInt2& uInt2);
UInt2 operator/(const UInt2& uInt1, const UInt2& uInt2);
UInt2 operator%(const UInt2& uInt1, const UInt2& uInt2);
							
struct UInt3
{
	UInt3(const unsigned int value = 0);
	UInt3(const unsigned int x , const unsigned int y, const unsigned int z);
	UInt3(const UInt2& uInt2);
	UInt3(const UInt4& uInt4);

	unsigned int x, y, z;
	void operator+=(const UInt3& int1);
	void operator-=(const UInt3& int1);
	void operator*=(const UInt3& int1);
	void operator/=(const UInt3& int1);
	void operator%=(const UInt3& int1);
	bool operator==(const UInt3& int1);
	bool operator!=(const UInt3& int1);
};
UInt3 operator+(const UInt3& uInt1, const UInt3& uInt2);
UInt3 operator-(const UInt3& uInt1, const UInt3& uInt2);
UInt3 operator*(const UInt3& uInt1, const UInt3& uInt2);
UInt3 operator/(const UInt3& uInt1, const UInt3& uInt2);
UInt3 operator%(const UInt3& uInt1, const UInt3& uInt2);
						
struct UInt4
{
	UInt4(const unsigned int value = 0);
	UInt4(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w);
	UInt4(const UInt2& uInt2);
	UInt4(const UInt3& uInt3);

	unsigned int x, y, z, w;
	void operator+=(const UInt4& int1);
	void operator-=(const UInt4& int1);
	void operator*=(const UInt4& int1);
	void operator/=(const UInt4& int1);
	void operator%=(const UInt4& int1);
	bool operator==(const UInt4& int1);
	bool operator!=(const UInt4& int1);
};
UInt4 operator+(const UInt4& uInt1, const UInt4& uInt2);
UInt4 operator-(const UInt4& uInt1, const UInt4& uInt2);
UInt4 operator*(const UInt4& uInt1, const UInt4& uInt2);
UInt4 operator/(const UInt4& uInt1, const UInt4& uInt2);
UInt4 operator%(const UInt4& uInt1, const UInt4& uInt2);



