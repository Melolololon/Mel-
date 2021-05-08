#include"Vector.h"


#pragma region Vector2

Vector2::Vector2()
{
	x = 0;
	y = 0;
}
Vector2::Vector2(const Vector3& v)
{
	x = v.x;
	y = v.y;
}
Vector2::Vector2(const float num)
{
	x = num;
	y = num;
}
Vector2::Vector2(const float x, const float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const DirectX::XMFLOAT2& f)
{
	x = f.x;
	y = f.y;
}

Vector2::Vector2(const DirectX::XMVECTOR& v)
{
	x = v.m128_f32[0];
	y = v.m128_f32[1];
}

#pragma region ââéZéq



Vector2 operator+(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x + vector2.x, vector1.y + vector2.y };
}
Vector2 operator-(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x - vector2.x,vector1.y - vector2.y };
}
Vector2 operator*(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x * vector2.x, vector1.y * vector2.y };
}
Vector2 operator/(const Vector2& vector1, const Vector2& vector2)
{
	return { vector1.x / vector2.x,vector1.y / vector2.y };
}


Vector2 operator+(const Vector2& vector, const float f)
{
	return { vector.x + f,vector.y + f };
}
Vector2 operator-(const Vector2& vector, const float f)
{
	return { vector.x - f, vector.y - f };
}
Vector2 operator*(const Vector2& vector, const float f)
{
	return { vector.x * f, vector.y * f };
}
Vector2 operator/(const Vector2& vector, const float f)
{
	return { vector.x / f,vector.y / f };
}

Vector2 operator+(const float f, const Vector2& vector)
{
	return { f + vector.x ,f + vector.y };
}

Vector2 operator-(const float f, const Vector2& vector)
{
	return { f - vector.x ,f - vector.y };
}

Vector2 operator*(const float f, const Vector2& vector)
{
	return { f * vector.x ,f * vector.y };
}

Vector2 operator/(const float f, const Vector2& vector)
{
	return { f / vector.x,f / vector.y };
}


void Vector2::operator+= (const Vector2& vector)
{
	x = x + vector.x;
	y = y + vector.y;
}

void Vector2::operator-= (const Vector2& vector)
{
	x = x - vector.x;
	y = y - vector.y;
}

void Vector2::operator*= (const Vector2& vector)
{
	x = x * vector.x;
	y = y * vector.y;
}

void Vector2::operator/= (const Vector2& vector)
{
	x = x / vector.x;
	y = y / vector.y;
}



void Vector2::operator=(const float num)
{
	x = num;
	y = num;
}

bool Vector2::operator==(const Vector2& vector)
{
	if (x == vector.x &&
		y == vector.y)
		return true;
	return false;
}

bool Vector2::operator!=(const Vector2& vector)
{
	if (x == vector.x &&
		y == vector.y)
		return false;
	return true;
}

void Vector2::operator++()
{
	x++;
	y++;
}
void Vector2::operator--()
{
	x--;
	y--;
}

#pragma endregion

#pragma region ä÷êî

float Vector2::Length(const Vector2& vector)
{
	return static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y ));
}

Vector2 Vector2::Normalize(const Vector2& vector)
{
	if (vector.x == 0 && vector.y == 0)return { 0,0 };
	float length = Length(vector);
	return{ vector.x / length ,vector.y / length };
}

float Vector2::Dot(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

float Vector2::Cross(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}


Vector2 Vector2Normalize(const Vector2& vector)
{
	if (vector.x == 0 && vector.y == 0)return { 0,0 };
	float length = static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y));
	return{ vector.x / length ,vector.y / length };
}


/// <summary>
/// ì‡êœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
float Vector2Dot(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y;
}

float Vector2Cross(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
#pragma endregion


#pragma endregion




#pragma region Vector3

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(const Vector2& v)
{
	x = v.x;
	y = v.y;
	z = 0;
}

Vector3::Vector3(const float num)
{
	x = num;
	y = num;
	z = num;
}

Vector3::Vector3(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(const DirectX::XMFLOAT3& f)
{
	x = f.x;
	y = f.y;
	z = f.z;
}


Vector3::Vector3(const DirectX::XMVECTOR& v)
{
	x = v.m128_f32[0];
	y = v.m128_f32[1];
	z = v.m128_f32[2];

}

#pragma region ââéZéq


Vector3 operator+(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x + vector2.x, vector1.y + vector2.y, vector1.z + vector2.z };
}
Vector3 operator-(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x - vector2.x,vector1.y - vector2.y, vector1.z - vector2.z };
}
Vector3 operator*(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x * vector2.x, vector1.y * vector2.y, vector1.z * vector2.z };
}
Vector3 operator/(const Vector3& vector1, const Vector3& vector2)
{
	return { vector1.x / vector2.x, vector1.y / vector2.y, vector1.z / vector2.z };
}

Vector3 operator+(const Vector3& vector, const float f)
{
	return { vector.x + f, vector.y + f,vector.z + f };
}
Vector3 operator-(const Vector3& vector, const float f)
{
	return { vector.x - f, vector.y - f, vector.z - f };
}
Vector3 operator*(const Vector3& vector, const float f)
{
	return { vector.x * f,vector.y * f, vector.z * f };
}
Vector3 operator/(const Vector3& vector, const float f)
{
	return { vector.x / f, vector.y / f, vector.z / f };
}


Vector3 operator+(const float f, const Vector3& vector)
{
	return { f + vector.x,f + vector.y,f + vector.z };
}
Vector3 operator-(const float f, const Vector3& vector)
{
	return { f - vector.x,f - vector.y,f - vector.z };
}
Vector3 operator*(const float f, const Vector3& vector)
{
	return { f * vector.x,f * vector.y,f * vector.z };
}
Vector3 operator/(const float f, const Vector3& vector)
{
	return { f / vector.x,f / vector.y,f / vector.z };
}


void Vector3::operator+= (const Vector3& vector)
{
	x = x + vector.x;
	y = y + vector.y;
	z = z + vector.z;
}

void Vector3::operator-= (const Vector3& vector)
{
	x = x - vector.x;
	y = y - vector.y;
	z = z - vector.z;
}

void Vector3::operator*= (const Vector3& vector)
{
	x = x * vector.x;
	y = y * vector.y;
	z = z * vector.z;
}

void Vector3::operator/= (const Vector3& vector)
{
	x = x / vector.x;
	y = y / vector.y;
	z = z / vector.z;
}

void Vector3::operator=(const float num)
{
	x = num;
	y = num;
	z = num;
}

bool Vector3::operator==(const Vector3& vector)
{
	if (x == vector.x &&
		y == vector.y &&
		z == vector.z)
		return true;
	return false;
}

bool Vector3::operator!=(const Vector3& vector)
{
	if (x == vector.x &&
		y == vector.y &&
		z == vector.z)
		return false;
	return true;
}


void Vector3::operator++()
{
	x++;
	y++;
	z++;
}
void Vector3::operator--()
{
	x--;
	y--;
	z--;
}
#pragma endregion

#pragma region ä÷êî
float Vector3::Length(const Vector3& vector) 
{
	return static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
}

Vector3 Vector3::Normalize(const Vector3& vector) 
{
	if (vector.x == 0 && vector.y == 0 && vector.z == 0)return {0,0,0};
	float length = Length(vector);
	return{ vector.x / length ,vector.y / length ,vector.z / length };
}

float Vector3::Dot(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}

Vector3 Vector3::Cross(const Vector3& vector1, const Vector3& vector2)
{
	Vector3 cross;
	cross.x = vector1.y * vector2.z - vector1.z * vector2.y;
	cross.y = vector1.z * vector2.x - vector1.x * vector2.z;
	cross.z = vector1.x * vector2.y - vector1.y * vector2.x;

	return cross;
}


/// <summary>
/// ê≥ãKâªÇµÇ‹Ç∑
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 Vector3Normalize(const Vector3& vector)
{
	if (vector.x == 0 && vector.y == 0 && vector.z == 0)return { 0,0,0 };
	float length = (float)(sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
	return{ vector.x / length ,vector.y / length ,vector.z / length };
}

/// <summary>
/// äOêœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
Vector3 Vector3Cross(const Vector3& vector1, const Vector3& vector2)
{
	Vector3 cross;
	cross.x = vector1.y * vector2.z - vector1.z * vector2.y;
	cross.y = vector1.z * vector2.x - vector1.x * vector2.z;
	cross.z = vector1.x * vector2.y - vector1.y * vector2.x;

	return cross;
}
/// <summary>
/// ì‡êœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
float Vector3Dot(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
#pragma endregion

#pragma endregion


