#include "Easing.h"

Vector3 Easing::EaseIn
(
	const float time,
	const Vector3& startPos,
	const Vector3& endPos
)
{
	return startPos * (1.0f - (time * time)) + endPos * (time * time);
}

Vector3 Easing::EaseOut(const float time, const Vector3& startPos, const Vector3& endPos)
{
	return startPos * (1.0f - (time * (2 - time))) + endPos * (time * (2 - time));
}
