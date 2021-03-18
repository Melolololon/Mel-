#pragma once
#include<DirectXMath.h>
struct Vector2  
{
	float x, y;

	Vector2();
	Vector2(const float& num);
	Vector2(const float& x, const float& y);
	Vector2(const DirectX::XMFLOAT2& f);

	DirectX::XMFLOAT2 toXMFLOAT2()const
	{
		return { x,y };
	}
#pragma region ââéZéq

	Vector2 operator+(const Vector2& vector);

	Vector2 operator-(const Vector2& vector);

	Vector2 operator*(const Vector2& vector);

	Vector2 operator/(const Vector2& vector);

	void operator+= (const Vector2& vector);

	void operator-= (const Vector2& vector);

	void operator*= (const Vector2& vector);

	void operator/= (const Vector2& vector);

	Vector2 operator*(const float& f);

	void operator=(const float&  num);
#pragma endregion


};


#pragma region ä÷êî

 float vector2Dot(const Vector2& vector1, const Vector2& vector2);
#pragma endregion



struct Vector3
{
	float x, y, z;

	Vector3();
	Vector3(const float& num);
	Vector3(const float& x, const float& y,const float& z);
	Vector3(const DirectX::XMFLOAT3& f);

	DirectX::XMFLOAT3 toXMFLOAT3() const
	{
		return { x,y ,z };
	}


#pragma region ââéZéq

	Vector3 operator+(const Vector3& vector);

	Vector3 operator-(const Vector3& vector);

	Vector3 operator*(const Vector3& vector);

	Vector3 operator/(const Vector3& vector);

	void operator+= (const Vector3& vector);

	void operator-= (const Vector3& vector);

	void operator*= (const Vector3& vector);

	void operator/= (const Vector3& vector);

	Vector3 operator*(const float& f);


	void operator=(const float& num);

#pragma endregion

};


#pragma region ä÷êî


 Vector3 vector3Normalize(const Vector3& vector);

 Vector3 vector3Cross(const Vector3& vector1, const Vector3& vector2);


 float vector3Dot(const Vector3& vector1, const Vector3& vector2);
#pragma endregion


