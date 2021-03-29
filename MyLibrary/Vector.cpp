#include"Vector.h"


#pragma region Vector2

Vector2::Vector2(){}
Vector2::Vector2(const Vector3& v)
{
	x = v.x;
	y = v.y;
}
Vector2::Vector2(const float& num)
{
	x = num;
	y = num;
}
Vector2::Vector2(const float& x, const float& y)
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

Vector2 Vector2::operator+(const Vector2& vector)const
{
	return { x + vector.x, y + vector.y };
}
Vector2 Vector2::operator-(const Vector2& vector)const
{
	return { x - vector.x, y - vector.y };
}
Vector2 Vector2::operator*(const Vector2& vector)const
{
	return { x * vector.x, y * vector.y };
}
Vector2 Vector2::operator/(const Vector2& vector)const
{
	return { x / vector.x, y / vector.y };
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


Vector2 Vector2::operator*(const float& f)const
{
	return { x * f, y * f };
}
Vector2 Vector2::operator+(const float& f)const
{
	return { x + f, y + f };
}
Vector2 Vector2::operator-(const float& f)const
{
	return { x - f, y - f };
}
Vector2 Vector2::operator/(const float& f)const
{
	return { x / f, y / f };
}

void Vector2::operator=(const float& num)
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

#pragma endregion

#pragma region ä÷êî

Vector2 vector2Normalize(const Vector2& vector)
{
	if (vector.x == 0 && vector.y == 0 )return { 0,0 };
	float size = static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y ));
	return{ vector.x / size ,vector.y / size };
}


/// <summary>
/// ì‡êœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
float vector2Dot(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x *vector2.x + vector1.y*vector2.y;
}

float vector2Cross(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.x * vector2.y - vector1.y * vector2.x;
}
#pragma endregion


#pragma endregion




#pragma region Vector3

Vector3::Vector3()
{
}

Vector3::Vector3(const Vector2& v)
{
	x = v.x;
	y = v.y;
	z = 0;
}

Vector3::Vector3(const float& num)
{
	x = num;
	y = num;
	z = num;
}

Vector3::Vector3(const float& x, const float& y, const float& z)
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

Vector3 Vector3::operator+(const Vector3&  vector)const
{
	return { x + vector.x, y + vector.y, z + vector.z };
}
Vector3 Vector3::operator-(const Vector3&  vector)const
{
	return { x - vector.x, y - vector.y, z - vector.z };
}
Vector3 Vector3::operator*(const Vector3&  vector)const
{
	return { x * vector.x, y * vector.y, z * vector.z };
}
Vector3 Vector3::operator/(const Vector3&  vector)const
{
	return { x / vector.x, y / vector.y, z / vector.z };
}


void Vector3::operator+= (const Vector3&  vector)
{
	x = x + vector.x;
	y = y + vector.y;
	z = z + vector.z;
}

void Vector3::operator-= (const Vector3&  vector)
{
	x = x - vector.x;
	y = y - vector.y;
	z = z - vector.z;
}

void Vector3::operator*= (const Vector3&  vector)
{
	x = x * vector.x;
	y = y * vector.y;
	z = z * vector.z;
}

void Vector3::operator/= (const Vector3&  vector)
{
	x = x / vector.x;
	y = y / vector.y;
	z = z / vector.z;
}


Vector3 Vector3::operator+(const float& f)const
{
	return { x + f, y + f, z +f };
}
Vector3 Vector3::operator-(const float& f)const
{
	return { x - f, y - f, z -f };
}
Vector3 Vector3::operator*(const float& f)const
{
	return { x * f, y * f, z *f };
}
Vector3 Vector3::operator/(const float& f)const
{
	return { x / f, y / f, z /f };
}

void Vector3::operator=(const float& num)
{
	x = num;
	y = num;
	z = num;
}

bool Vector3::operator==(const Vector3& vector)
{
	if (x == vector.x && 
		y == vector.y &&
		z == vector.z )
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


#pragma endregion

#pragma region ä÷êî

/// <summary>
/// ê≥ãKâªÇµÇ‹Ç∑
/// </summary>
/// <param name="vector"></param>
/// <returns></returns>
Vector3 vector3Normalize(const Vector3& vector)
{
	if (vector.x == 0 && vector.y == 0 && vector.z == 0)return { 0,0,0 };
	float size = (float)(sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
	return{ vector.x / size ,vector.y / size ,vector.z / size };
}

/// <summary>
/// äOêœÇãÅÇﬂÇ‹Ç∑
/// </summary>
/// <param name="vector1"></param>
/// <param name="vector2"></param>
/// <returns></returns>
Vector3 vector3Cross(const Vector3& vector1, const Vector3& vector2)
{
	Vector3 cross;
	cross.x = vector1.y *vector2.z - vector1.z * vector2.y;
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
float vector3Dot(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.x *vector2.x + vector1.y*vector2.y + vector1.z *vector2.z;
}
#pragma endregion

#pragma endregion





#pragma region Vector4

#pragma region ââéZéq

Vector4 Vector4::operator+(Vector4 vector)
{
	return { x + vector.x, y + vector.y, z + vector.z,w + vector.w };
}
Vector4 Vector4::operator-(Vector4 vector)
{
	return { x - vector.x, y - vector.y, z - vector.z ,w - vector.w };
}
Vector4 Vector4::operator*(Vector4 vector)
{
	return { x * vector.x, y * vector.y, z * vector.z ,w * vector.w };
}
Vector4 Vector4::operator/(Vector4 vector)
{
	return { x / vector.x, y / vector.y, z / vector.z ,w / vector.w };
}

void Vector4::operator+= (Vector4 vector)
{
	x = x + vector.x;
	y = y + vector.y;
	z = z + vector.z;
	w = w + vector.w;
}

void Vector4::operator-= (Vector4 vector)
{
	x = x - vector.x;
	y = y - vector.y;
	z = z - vector.z;
	w = w - vector.w;
}

void Vector4::operator*= (Vector4 vector)
{
	x = x * vector.x;
	y = y * vector.y;
	z = z * vector.z;
	w = w * vector.w;
}

void Vector4::operator/= (Vector4 vector)
{
	x = x / vector.x;
	y = y / vector.y;
	z = z / vector.z;
	w = w / vector.w;
}


Vector4 Vector4::operator*(float f)
{
	return { x * f, y * f, z *f ,w * f };
}

void Vector4::operator=(float num)
{
	x = num;
	y = num;
	z = num;
	w = num;
}



#pragma endregion

#pragma endregion

