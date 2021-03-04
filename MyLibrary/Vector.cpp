#include"Vector.h"


#pragma region Vector2

#pragma region ââéZéq

Vector2 Vector2::operator+(Vector2 vector)
{
	return { x + vector.x, y + vector.y };
}
Vector2 Vector2::operator-(Vector2 vector)
{
	return { x - vector.x, y - vector.y };
}
Vector2 Vector2::operator*(Vector2 vector)
{
	return { x * vector.x, y * vector.y };
}
Vector2 Vector2::operator/(Vector2 vector)
{
	return { x / vector.x, y / vector.y };
}

void Vector2::operator+= (Vector2 vector)
{
	x = x + vector.x;
	y = y + vector.y;
}

void Vector2::operator-= (Vector2 vector)
{
	x = x - vector.x;
	y = y - vector.y;
}

void Vector2::operator*= (Vector2 vector)
{
	x = x * vector.x;
	y = y * vector.y;
}

void Vector2::operator/= (Vector2 vector)
{
	x = x / vector.x;
	y = y / vector.y;
}


Vector2 Vector2::operator*(float f)
{
	return { x * f, y * f };
}

void Vector2::operator=(float num)
{
	x = num;
	y = num;
}

#pragma endregion

#pragma region ä÷êî

float Vector2::dot(Vector2 vector1, Vector2 vector2)
{
	return vector1.x *vector2.x + vector1.y*vector2.y;
}
#pragma endregion


#pragma endregion




#pragma region Vector3

#pragma region ââéZéq

Vector3 Vector3::operator+(Vector3 vector)
{
	return { x + vector.x, y + vector.y, z + vector.z };
}
Vector3 Vector3::operator-(Vector3 vector)
{
	return { x - vector.x, y - vector.y, z - vector.z };
}
Vector3 Vector3::operator*(Vector3 vector)
{
	return { x * vector.x, y * vector.y, z * vector.z };
}
Vector3 Vector3::operator/(Vector3 vector)
{
	return { x / vector.x, y / vector.y, z / vector.z };
}


void Vector3::operator+= (Vector3 vector)
{
	x = x + vector.x;
	y = y + vector.y;
	z = z + vector.z;
}

void Vector3::operator-= (Vector3 vector)
{
	x = x - vector.x;
	y = y - vector.y;
	z = z - vector.z;
}

void Vector3::operator*= (Vector3 vector)
{
	x = x * vector.x;
	y = y * vector.y;
	z = z * vector.z;
}

void Vector3::operator/= (Vector3 vector)
{
	x = x / vector.x;
	y = y / vector.y;
	z = z / vector.z;
}


Vector3 Vector3::operator*(float f)
{
	return { x * f, y * f, z *f };
}

void Vector3::operator=(float num)
{
	x = num;
	y = num;
	z = num;
}

#pragma endregion

#pragma region ä÷êî

Vector3 Vector3::straight(Vector3 myPos, Vector3 otherPos)
{
	Vector3 vector;
	vector.x = (otherPos.x - myPos.x);
	vector.y = (otherPos.y - myPos.y);
	vector.z = (otherPos.z - myPos.z);
	float size = (float)(sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
	return{ vector.x / size ,vector.y / size ,vector.z / size };
}

Vector3 Vector3::normalize(Vector3 vector)
{
	if (vector.x == 0 && vector.y == 0 && vector.z == 0)return { 0,0,0 };
	float size = (float)(sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z));
	return{ vector.x / size ,vector.y / size ,vector.z / size };
}

Vector3 Vector3::cross(Vector3 vector1, Vector3 vector2)
{
	Vector3 cross;
	cross.x = vector1.y *vector2.z - vector1.z * vector2.y;
	cross.y = vector1.z * vector2.x - vector1.x * vector2.z;
	cross.z = vector1.x * vector2.y - vector1.y * vector2.x;

	return cross;
}

float Vector3::dot(Vector3 vector1, Vector3 vector2)
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
