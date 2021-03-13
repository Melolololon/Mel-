#pragma once
#include<DirectXMath.h>
struct Vector2  
{
	float x, y;

	Vector2();
	Vector2(const float& num);
	Vector2(const float& x, const float& y);
	Vector2(const DirectX::XMFLOAT2& f);

	DirectX::XMFLOAT2 toXMFLOAT2()
	{
		return { x,y };
	}
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

	Vector3();
	Vector3(const float& num);
	Vector3(const float& x, const float& y,const float& z);
	Vector3(const DirectX::XMFLOAT3& f);

	DirectX::XMFLOAT3 toXMFLOAT3()
	{
		return { x,y ,z };
	}
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

	Vector4();
	Vector4(const float& num);
	Vector4(const float& x, const float& y, const float& z, const float& w);
	Vector4(const DirectX::XMFLOAT4& f);

	DirectX::XMFLOAT4 toXMFLOAT4()
	{
		return { x,y ,z ,w};
	}

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