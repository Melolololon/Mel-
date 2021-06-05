#include "Easing.h"

Vector3 Easing::EaseIn
(
	const float time,
	const Vector3& startPos,
	const Vector3& endPos
)
{
	//ç∑
	Vector3 dis = 0;
	/*dis.x = abs(endPos.x - startPos.x);
	dis.y = abs(endPos.y - startPos.y);
	dis.z = abs(endPos.z - startPos.z);*/

	return dis * (time * time);

}
