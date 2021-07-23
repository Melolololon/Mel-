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

#pragma region ���Z�q



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

Vector2 Vector2::operator-()
{
	Vector2 v = Vector2(x, y);
	return v * -1;
}


#pragma endregion

#pragma region �֐�
float Vector2::Length()const
{
	return static_cast<float>(sqrt(x * x + y * y));
}

float Vector2::Length(const Vector2& vector)
{
	return vector.Length();
}

Vector2 Vector2::Normalize()const
{
	if (x == 0 && y == 0)return 0;
	float length = Length(Vector2(x, y));
	return Vector2(x / length, y / length);
}

Vector2 Vector2::Normalize(const Vector2& vector)
{
	return vector.Normalize();
}

float Vector2::Dot(const Vector2& vector)const
{
	return x * vector.x + y * vector.y;
}
float Vector2::Dot(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.Dot(vector2);
}


float Vector2::Cross(const Vector2& vector)const
{
	return x * vector.y - y * vector.x;
}
float Vector2::Cross(const Vector2& vector1, const Vector2& vector2)
{
	return vector1.Cross(vector2);
}


Vector2 Vector2::Abs()const
{
	Vector2 returnVector = Vector2(x, y);
	if (returnVector.x < 0)returnVector.x *= -1;
	if (returnVector.y < 0)returnVector.y *= -1;
	return returnVector;
}
Vector2 Vector2::Abs(const Vector2& vector)
{
	return vector.Abs();
}

Vector2 Vector2Normalize(const Vector2& vector)
{
	if (vector.x == 0 && vector.y == 0)return { 0,0 };
	float length = static_cast<float>(sqrt(vector.x * vector.x + vector.y * vector.y));
	return{ vector.x / length ,vector.y / length };
}


/// <summary>
/// ���ς����߂܂�
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

#pragma region ���Z�q


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

Vector3 Vector3::operator-()
{
	Vector3 vector = Vector3(x, y, z);
	return vector * -1;
}

#pragma endregion

#pragma region �֐�
float Vector3::Length()const
{
	return static_cast<float>(sqrt(x * x + y * y + z * z));
}
float Vector3::Length(const Vector3& vector) 
{
	return vector.Length();
}

Vector3 Vector3::Normalize() const
{
	if (x == 0 && y == 0 && z == 0)return 0;
	float length = Vector3(x, y, z).Length();
	return Vector3(x / length ,y / length ,z / length );
}

Vector3 Vector3::Normalize(const Vector3& vector) 
{
	return vector.Normalize();
}

float Vector3::Dot(const Vector3& vector)const
{
	return x * vector.x + y * vector.y + z * vector.z;
}
float Vector3::Dot(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.Dot(vector2);
}

Vector3 Vector3::Cross(const Vector3& vector)const
{
	Vector3 cross;
	cross.x = y * vector.z - z * vector.y;
	cross.y = z * vector.x - x * vector.z;
	cross.z = x * vector.y - y * vector.x;

	return cross;
}


Vector3 Vector3::Cross(const Vector3& vector1, const Vector3& vector2)
{
	return vector1.Cross(vector2);
}

Vector3 Vector3::Abs()const
{
	Vector3 returnVector = Vector3(x, y, z);
	if (returnVector.x < 0)returnVector.x *= -1;
	if (returnVector.y < 0)returnVector.y *= -1;
	if (returnVector.z < 0)returnVector.z *= -1;
	return returnVector;
}
Vector3 Vector3::Abs(const Vector3& vector)
{
	return vector.Abs();
}


/// <summary>
/// ���K�����܂�
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
/// �O�ς����߂܂�
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
/// ���ς����߂܂�
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


