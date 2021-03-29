#pragma once
#include<DirectXMath.h>
struct Vector2;
struct Vector3;

struct Vector2  
{
	float x, y;

	Vector2();
	Vector2(const Vector3& v);
	Vector2(const float& num);
	Vector2(const float& x, const float& y);
	Vector2(const DirectX::XMFLOAT2& f);
	Vector2(const DirectX::XMVECTOR& v);

	DirectX::XMFLOAT2 toXMFLOAT2()const
	{
		return { x,y };
	}
#pragma region ââéZéq

	Vector2 operator+(const Vector2& vector)const;

	Vector2 operator-(const Vector2& vector)const;

	Vector2 operator*(const Vector2& vector)const;

	Vector2 operator/(const Vector2& vector)const;

	void operator+= (const Vector2& vector);

	void operator-= (const Vector2& vector);

	void operator*= (const Vector2& vector);

	void operator/= (const Vector2& vector);

	Vector2 operator*(const float& f)const;
	Vector2 operator+(const float& f)const;
	Vector2 operator-(const float& f)const;
	Vector2 operator/(const float& f)const;

	void operator=(const float&  num);
	bool operator==(const Vector2& vector);
	bool operator!=(const Vector2& vector);

#pragma endregion


};


#pragma region ä÷êî

Vector2 vector2Normalize(const Vector2& vector);

float vector2Dot(const Vector2& vector1, const Vector2& vector2);

float vector2Cross(const Vector2& vector1, const Vector2& vector2);

#pragma endregion



struct Vector3
{
	float x, y, z;

	Vector3();
	Vector3(const Vector2& v);
	Vector3(const float& num);
	Vector3(const float& x, const float& y,const float& z);
	Vector3(const DirectX::XMFLOAT3& f);
	Vector3(const DirectX::XMVECTOR& v);

	DirectX::XMFLOAT3 toXMFLOAT3() const
	{
		return { x,y ,z };
	}


#pragma region ââéZéq

	Vector3 operator+(const Vector3& vector) const;

	Vector3 operator-(const Vector3& vector)const;

	Vector3 operator*(const Vector3& vector)const;

	Vector3 operator/(const Vector3& vector)const;

	void operator+= (const Vector3& vector);

	void operator-= (const Vector3& vector);

	void operator*= (const Vector3& vector);

	void operator/= (const Vector3& vector);

	Vector3 operator+(const float& f)const;
	Vector3 operator-(const float& f)const;
	Vector3 operator*(const float& f)const;
	Vector3 operator/(const float& f)const;


	void operator=(const float& num);
	bool operator==(const Vector3& vector);
	bool operator!=(const Vector3& vector);
	
#pragma endregion

};


#pragma region ä÷êî


Vector3 vector3Normalize(const Vector3& vector);

 Vector3 vector3Cross(const Vector3& vector1, const Vector3& vector2);

 float vector3Dot(const Vector3& vector1, const Vector3& vector2);
#pragma endregion


 struct Vector4
 {
	 float x, y, z, w;

#pragma region ââéZéq

	 Vector4 operator+(Vector4 vector);

	 Vector4 operator-(Vector4 vector);

	 Vector4 operator*(Vector4 vector);

	 Vector4 operator/(Vector4 vector);

	 void operator+= (Vector4 vector);

	 void operator-= (Vector4 vector);

	 void operator*= (Vector4 vector);

	 void operator/= (Vector4 vector);

	 Vector4 operator*(float f);

	 void operator=(float num);

#pragma endregion

 };

