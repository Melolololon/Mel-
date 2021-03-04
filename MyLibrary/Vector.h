#pragma once
#include<DirectXMath.h>
struct Vector2  
{
	float x, y;

#pragma region 演算子

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

#pragma region 関数


	/// <summary>
	/// 内積を求めます
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

#pragma region 演算子

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

#pragma region 関数

	/// <summary>
/// 自分から相手へのベクトルを取得します
/// </summary>
/// <param name="myPos">自分の座標</param>
/// <param name="otherPos">相手の座標</param>
/// <returns></returns>
	static Vector3 straight(Vector3 myPos, Vector3 otherPos);

	/// <summary>
	/// 正規化します
	/// </summary>
	/// <param name="vector"></param>
	/// <returns></returns>
	static Vector3 normalize(Vector3 vector);

	/// <summary>
	/// 外積を求めます
	/// </summary>
	/// <param name="vector1"></param>
	/// <param name="vector2"></param>
	/// <returns></returns>
	static Vector3 cross(Vector3 vector1, Vector3 vector2);

	/// <summary>
	/// 内積を求めます
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

#pragma region 演算子

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