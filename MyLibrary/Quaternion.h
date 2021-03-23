#pragma once
#include"Vector.h"
struct Quaternion
{
	float w, x, y, z;

	Quaternion();
	Quaternion(const float& w,const float& x, const float& y, const float& z);
	Quaternion(const float& w,const Vector3& v);
	Quaternion(const float& w,const Vector2& v);
	Quaternion(const Vector3& v);
	Quaternion(const Vector2& v);
	Quaternion operator* (const Quaternion& q)const;
	
	void operator*= (const Quaternion& q);
};

Quaternion getRotateQuaternion(const Vector3& pos, const Vector3& vector, const float& angle);
