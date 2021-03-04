#pragma once
#include<DirectXMath.h>
struct Vector2  
{
	float x, y;

#pragma region ���Z�q

	Vector2 operator+(Vector2 vector);

	Vector2 operator-(Vector2 vector);

	Vector2 operator*(Vector2 vector);

	Vector2 operator/(Vector2 vector);

	void operator+= (Vector2 vector);

	void operator-= (Vector2 vector);

	void operator*= (Vector2 vector);

	void operator/= (Vector2 vector);

	Vector2 operator*(float f);

	void operator=(float num);
#pragma endregion

#pragma region �֐�


	/// <summary>
	/// ���ς����߂܂�
	/// </summary>
	/// <param name="vector1"></param>
	/// <param name="vector2"></param>
	/// <returns></returns>
	static float dot(Vector2 vector1, Vector2 vector2);
#pragma endregion

};


struct Vector3
{
	float x, y, z;

#pragma region ���Z�q

	Vector3 operator+(Vector3 vector);

	Vector3 operator-(Vector3 vector);

	Vector3 operator*(Vector3 vector);

	Vector3 operator/(Vector3 vector);

	void operator+= (Vector3 vector);

	void operator-= (Vector3 vector);

	void operator*= (Vector3 vector);

	void operator/= (Vector3 vector);

	Vector3 operator*(float f);


	void operator=(float num);

#pragma endregion

#pragma region �֐�

	/// <summary>
/// �������瑊��ւ̃x�N�g�����擾���܂�
/// </summary>
/// <param name="myPos">�����̍��W</param>
/// <param name="otherPos">����̍��W</param>
/// <returns></returns>
	static Vector3 straight(Vector3 myPos, Vector3 otherPos);

	/// <summary>
	/// ���K�����܂�
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	static Vector3 normalize(Vector3 vector);

	/// <summary>
	/// �O�ς����߂܂�
	/// </summary>
	/// <param name="vector1"></param>
	/// <param name="vector2"></param>
	/// <returns></returns>
	static Vector3 cross(Vector3 vector1, Vector3 vector2);

	/// <summary>
	/// ���ς����߂܂�
	/// </summary>
	/// <param name="vector1"></param>
	/// <param name="vector2"></param>
	/// <returns></returns>
	static float dot(Vector3 vector1, Vector3 vector2);
#pragma endregion

};


struct Vector4 
{
	float x, y, z, w;

#pragma region ���Z�q

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