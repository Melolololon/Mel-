#include "Collision.h"

bool Collision::RectAndRect(const RectData& rect1, const RectData& rect2)
{
	return rect2.size.x + rect2.position.x > rect1.position.x &&
		rect2.position.x < rect1.size.x + rect2.position.x &&
		rect2.position.y < rect1.size.y + rect1.position.y &&
		rect2.size.y + rect2.position.y > rect1.position.y;
}

bool Collision::CircleAndCircle(const CircleData& circle1, const CircleData& circle2)
{
	return ((circle1.position.x - circle2.position.x) *
		(circle1.position.x - circle2.position.x) +
		(circle1.position.y - circle2.position.y) *
		(circle1.position.y - circle2.position.y)) <=
		((circle1.r + circle2.r) * (circle1.r + circle2.r));
}

bool Collision::RectAndCircle(const RectData& rect, const CircleData& circle)
{
	return false;
}

bool Collision::CircleAndLineSegment2D(const CircleData& circle, const LineSegment2DData& lineSegment)
{
	return false;
}



