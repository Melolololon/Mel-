#include "Quaternion.h"
#include<cmath>
#include"LibMath.h"

Quaternion::Quaternion(){}

Quaternion::Quaternion(const float w, const float x, const float y, const float z)
{
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::Quaternion(const float w, const Vector3& v)
{
	this->w = w;
	x = v.x;
	y = v.y;
	z = v.z;
}

Quaternion::Quaternion(const float w, const Vector2& v)
{
	this->w = w;
	x = v.x;
	y = v.y;
	z = 0;
}

Quaternion::Quaternion(const Vector3& v)
{
	w = 0;
	x = v.x;
	y = v.y;
	z = v.z;
}

Quaternion::Quaternion(const Vector2& v)
{
	w = 0;
	x = v.x;
	y = v.y;
	z = 0;
}

Quaternion Quaternion::operator* (const Quaternion& q)const
{
	return 
	{
		-x * q.x - y * q.y - z * q.z + w * q.w,
		 x * q.w + y * q.z - z * q.y + w * q.x,
		-x * q.z + y * q.w + z * q.x + w * q.y,
		 x * q.y - y * q.x + z * q.w + w * q.z
	};

}

void Quaternion::operator*= (const Quaternion& q)
{
	Quaternion p(w, x, y, z);
	p = p * q;

	x = p.x;
	y = p.y;
	z = p.z;
	w = p.w;
}


Quaternion GetRotateQuaternion(const Vector3& pos, const Vector3& vector, const float angle)
{
	float rad = LibMath::AngleConversion(0, angle);
	Vector3 nVector = Vector3Normalize(vector);//ê≥ãKâª
	Quaternion p(0.0f, pos);
	Quaternion q
	(
		std::cos(rad / 2),
		nVector.x * std::sin(rad / 2),
		nVector.y * std::sin(rad / 2),
		nVector.z * std::sin(rad / 2)
	);

	Quaternion r
	(
		std::cos(rad / 2),
		-nVector.x * std::sin(rad / 2),
		-nVector.y * std::sin(rad / 2),
		-nVector.z * std::sin(rad / 2)
	);
	return r * p * q;

}
