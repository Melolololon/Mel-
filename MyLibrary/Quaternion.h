#pragma once
struct Quaternion
{
	float x, y, z, w;

	Quaternion(const float& x, const float& y, const float& z, const float& w);
	Quaternion operator* (const Quaternion& q);
};

