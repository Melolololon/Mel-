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

Int2::Int2(const Int3& int3)
{
	x = int3.x;
	y = int3.y;
}

Int2::Int2(const Int4& int4)
{
	x = int4.x;
	y = int4.y;
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


#pragma region Int3
Int3::Int3(const int value)
{
	x = value;
	y = value;
	z = value;
}

Int3::Int3(const int x, const int y, const int z)
{
	this->x = x;
	this->y = y;
	this->z = z;

}

Int3::Int3(const Int2& int2)
{
	x = int2.x;
	y = int2.y;
	z = 0;
}

Int3::Int3(const Int4& int4)
{
	x = int4.x;
	y = int4.y;
	z = int4.z;
}



void Int3::operator+=(const Int3& int1)
{
	x = x + int1.x;
	y = y + int1.y;
	z = z + int1.z;
}

void Int3::operator-=(const Int3& int1)
{
	x = x - int1.x;
	y = y - int1.y;
	z = z - int1.z;
}

void Int3::operator*=(const Int3& int1)
{
	x = x * int1.x;
	y = y * int1.y;
	z = z * int1.z;
}

void Int3::operator/=(const Int3& int1)
{
	x = x / int1.x;
	y = y / int1.y;
	z = z / int1.z;
}

void Int3::operator%=(const Int3& int1)
{
	x = x % int1.x;
	y = y % int1.y;
	z = z % int1.z;
}

bool Int3::operator==(const Int3& int1)
{
	return x == int1.x && y == int1.y && z == int1.z;
}

bool Int3::operator!=(const Int3& int1)
{
	return x != int1.x || y != int1.y || z != int1.z;
}



Int3 operator+(const Int3& int1, const Int3& int2)
{
	return Int3(int1.x + int2.x, int1.y + int2.y, int1.z + int2.z);
}

Int3 operator-(const Int3& int1, const Int3& int2)
{
	return Int3(int1.x - int2.x, int1.y - int2.y, int1.z - int2.z);
}

Int3 operator*(const Int3& int1, const Int3& int2)
{
	return Int3(int1.x * int2.x, int1.y * int2.y, int1.z * int2.z);
}

Int3 operator/(const Int3& int1, const Int3& int2)
{
	return Int3(int1.x / int2.x, int1.y / int2.y, int1.z / int2.z);
}

Int3 operator%(const Int3& int1, const Int3& int2)
{
	return Int3(int1.x % int2.x, int1.y % int2.y, int1.z % int2.z);
}
#pragma endregion


#pragma region Int4


Int4::Int4(const int value)
{
	x = value;
	y = value;
	z = value;
	w = value;
}

Int4::Int4(const int x, const int y, const int z, const int w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Int4::Int4(const Int2& int2)
{
	x = int2.x;
	y = int2.y;
	z = 0;
	w = 0;
}

Int4::Int4(const Int3& int3)
{
	x = int3.x;
	y = int3.y;
	z = int3.z;
	w = 0;
}




void Int4::operator+=(const Int4& int1)
{
	x = x + int1.x;
	y = y + int1.y;
	z = z + int1.z;
	w = w + int1.w;
}

void Int4::operator-=(const Int4& int1)
{
	x = x - int1.x;
	y = y - int1.y;
	z = z - int1.z;
	w = w - int1.w;
}

void Int4::operator*=(const Int4& int1)
{
	x = x * int1.x;
	y = y * int1.y;
	z = z * int1.z;
	w = w * int1.w;
}

void Int4::operator/=(const Int4& int1)
{
	x = x / int1.x;
	y = y / int1.y;
	z = z / int1.z;
	w = w / int1.w;
}

void Int4::operator%=(const Int4& int1)
{
	x = x % int1.x;
	y = y % int1.y;
	z = z % int1.z;
	w = w % int1.w;
}

bool Int4::operator==(const Int4& int1)
{
	return x == int1.x && y == int1.y && z == int1.z && w == int1.w;
}

bool Int4::operator!=(const Int4& int1)
{
	return x != int1.x || y != int1.y || z != int1.z || w != int1.w;
}



Int4 operator+(const Int4& int1, const Int4& int2)
{
	return Int4(int1.x + int2.x, int1.y + int2.y, int1.z + int2.z, int1.w + int2.w);
}

Int4 operator-(const Int4& int1, const Int4& int2)
{
	return Int4(int1.x - int2.x, int1.y - int2.y, int1.z - int2.z, int1.w - int2.w);
}

Int4 operator*(const Int4& int1, const Int4& int2)
{
	return Int4(int1.x * int2.x, int1.y * int2.y, int1.z * int2.z, int1.w * int2.w);
}

Int4 operator/(const Int4& int1, const Int4& int2)
{
	return Int4(int1.x / int2.x, int1.y / int2.y, int1.z / int2.z, int1.w / int2.w);
}

Int4 operator%(const Int4& int1, const Int4& int2)
{
	return Int4(int1.x % int2.x, int1.y % int2.y, int1.z % int2.z, int1.w % int2.w);
}
#pragma endregion


#pragma region UInt2

UInt2::UInt2(const unsigned int value)
{
	x = value;
	y = value;
}


UInt2::UInt2(const unsigned int x, const unsigned int y)
{
	this->x = x;
	this->y = y;
}

UInt2::UInt2(const UInt3& uInt3)
{
	x = uInt3.x;
	y = uInt3.y;
}

UInt2::UInt2(const UInt4& uInt4)
{
	x = uInt4.x;
	y = uInt4.y;
}



void UInt2::operator+=(const UInt2& int1)
{
	x = x + int1.x;
	y = y + int1.y;
}

void UInt2::operator-=(const UInt2& int1)
{
	x = x - int1.x;
	y = y - int1.y;
}

void UInt2::operator*=(const UInt2& int1)
{
	x = x * int1.x;
	y = y * int1.y;
}

void UInt2::operator/=(const UInt2& int1)
{
	x = x / int1.x;
	y = y / int1.y;
}

void UInt2::operator%=(const UInt2& int1)
{
	x = x % int1.x;
	y = y % int1.y;
}

bool UInt2::operator==(const UInt2& int1)
{
	return x == int1.x && y == int1.y;
}

bool UInt2::operator!=(const UInt2& int1)
{
	return x != int1.x || y != int1.y;
}



UInt2 operator+(const UInt2& int1, const UInt2& int2)
{
	return UInt2(int1.x + int2.x, int1.y + int2.y);
}

UInt2 operator-(const UInt2& int1, const UInt2& int2)
{
	return UInt2(int1.x - int2.x, int1.y - int2.y);
}

UInt2 operator*(const UInt2& int1, const UInt2& int2)
{
	return UInt2(int1.x * int2.x, int1.y * int2.y);
}

UInt2 operator/(const UInt2& int1, const UInt2& int2)
{
	return UInt2(int1.x / int2.x, int1.y / int2.y);
}

UInt2 operator%(const UInt2& int1, const UInt2& int2)
{
	return UInt2(int1.x % int2.x, int1.y % int2.y);
}

#pragma endregion

#pragma region UInt3



UInt3::UInt3(const unsigned int value)
{
	x = value;
	y = value;
	z = value;
}
UInt3::UInt3(const unsigned int x, const unsigned int y, const unsigned int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

UInt3::UInt3(const UInt2& uInt2)
{
	x = uInt2.x;
	y = uInt2.y;
	z = 0;
}

UInt3::UInt3(const UInt4& uInt4)
{
	x = uInt4.x;
	y = uInt4.y;
	z = uInt4.z;
}





void UInt3::operator+=(const UInt3& int1)
{
	x = x + int1.x;
	y = y + int1.y;
	z = z + int1.z;
}

void UInt3::operator-=(const UInt3& int1)
{
	x = x - int1.x;
	y = y - int1.y;
	z = z - int1.z;
}

void UInt3::operator*=(const UInt3& int1)
{
	x = x * int1.x;
	y = y * int1.y;
	z = z * int1.z;
}

void UInt3::operator/=(const UInt3& int1)
{
	x = x / int1.x;
	y = y / int1.y;
	z = z / int1.z;
}

void UInt3::operator%=(const UInt3& int1)
{
	x = x % int1.x;
	y = y % int1.y;
	z = z % int1.z;
}

bool UInt3::operator==(const UInt3& int1)
{
	return x == int1.x && y == int1.y && z == int1.z;
}

bool UInt3::operator!=(const UInt3& int1)
{
	return x != int1.x || y != int1.y || z != int1.z;
}



UInt3 operator+(const UInt3& int1, const UInt3& int2)
{
	return UInt3(int1.x + int2.x, int1.y + int2.y, int1.z + int2.z);
}

UInt3 operator-(const UInt3& int1, const UInt3& int2)
{
	return UInt3(int1.x - int2.x, int1.y - int2.y, int1.z - int2.z);
}

UInt3 operator*(const UInt3& int1, const UInt3& int2)
{
	return UInt3(int1.x * int2.x, int1.y * int2.y, int1.z * int2.z);
}

UInt3 operator/(const UInt3& int1, const UInt3& int2)
{
	return UInt3(int1.x / int2.x, int1.y / int2.y, int1.z / int2.z);
}

UInt3 operator%(const UInt3& int1, const UInt3& int2)
{
	return UInt3(int1.x % int2.x, int1.y % int2.y, int1.z % int2.z);
}

#pragma endregion

#pragma region UInt4



UInt4::UInt4(const unsigned int value)
{
	x = value;
	y = value;
	z = value;
	w = value;
}
UInt4::UInt4(const unsigned int x, const unsigned int y, const unsigned int z, const unsigned int w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

UInt4::UInt4(const UInt2& uInt2)
{
	x = uInt2.x;
	y = uInt2.y;
	z = 0;
	w = 0;
}

UInt4::UInt4(const UInt3& uInt3)
{
	x = uInt3.x;
	y = uInt3.y;
	z = uInt3.z;
	w = 0;
}



void UInt4::operator+=(const UInt4& int1)
{
	x = x + int1.x;
	y = y + int1.y;
	z = z + int1.z;
	w = w + int1.w;
}

void UInt4::operator-=(const UInt4& int1)
{
	x = x - int1.x;
	y = y - int1.y;
	z = z - int1.z;
	w = w - int1.w;
}

void UInt4::operator*=(const UInt4& int1)
{
	x = x * int1.x;
	y = y * int1.y;
	z = z * int1.z;
	w = w * int1.w;
}

void UInt4::operator/=(const UInt4& int1)
{
	x = x / int1.x;
	y = y / int1.y;
	z = z / int1.z;
	w = w / int1.w;
}

void UInt4::operator%=(const UInt4& int1)
{
	x = x % int1.x;
	y = y % int1.y;
	z = z % int1.z;
	w = w % int1.w;
}

bool UInt4::operator==(const UInt4& int1)
{
	return x == int1.x && y == int1.y && z == int1.z && w == int1.w;
}

bool UInt4::operator!=(const UInt4& int1)
{
	return x != int1.x || y != int1.y || z != int1.z || w != int1.w;
}



UInt4 operator+(const UInt4& int1, const UInt4& int2)
{
	return UInt4(int1.x + int2.x, int1.y + int2.y, int1.z + int2.z, int1.w + int2.w);
}

UInt4 operator-(const UInt4& int1, const UInt4& int2)
{
	return UInt4(int1.x - int2.x, int1.y - int2.y, int1.z - int2.z, int1.w - int2.w);
}

UInt4 operator*(const UInt4& int1, const UInt4& int2)
{
	return UInt4(int1.x * int2.x, int1.y * int2.y, int1.z * int2.z, int1.w * int2.w);
}

UInt4 operator/(const UInt4& int1, const UInt4& int2)
{
	return UInt4(int1.x / int2.x, int1.y / int2.y, int1.z / int2.z, int1.w / int2.w);
}

UInt4 operator%(const UInt4& int1, const UInt4& int2)
{
	return UInt4(int1.x % int2.x, int1.y % int2.y, int1.z % int2.z, int1.w % int2.w);
}
#pragma endregion