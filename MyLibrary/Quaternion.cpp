#include "Quaternion.h"

Quaternion::Quaternion(const float& x, const float& y, const float& z, const float& w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion Quaternion::operator* (const Quaternion& q)
{
	return 
	{
		w * q.x - z * q.y + y * q.z + x * q.w,
		z * q.x + w * q.y - x * q.z + y * q.w,
		-y * q.x + x * q.y + w * q.z + z * q.w,
		-x * q.x - y * q.y - z * q.z + w * q.w
	};

}
