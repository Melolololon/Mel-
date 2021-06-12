#pragma once
#include"Vector.h"
class Easing
{
private:
	Easing();
	~Easing();

public:
	static Vector3 EaseIn
	(
		const float time,
		const Vector3& startPos,
		const Vector3& endPos
	);
	
	static Vector3 EaseOut
	(
		const float time,
		const Vector3& startPos,
		const Vector3& endPos
	);

};

