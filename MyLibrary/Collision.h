#pragma once
#include"CollisionType.h"

//“–‚½‚è”»’è(‚ÌŒvŽZ)ƒNƒ‰ƒX
class Collision
{
	static bool RectAndRect(const RectData& rect1, const RectData& rect2);
	static bool CircleAndCircle(const CircleData& circle1, const CircleData& circle2);
	static bool RectAndCircle(const RectData& rect, const CircleData& circle);
	static bool CircleAndLineSegment2D(const CircleData& circle, const LineSegment2DData& lineSegment);

	static bool SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2);
	static bool BoxAndBox(const BoxData& box1, const BoxData& box2);
	static bool LineSegment3DAndLineSegment3D(const LineSegment3DData& lineSegment1 , const LineSegment3DData& lineSegment2);
	static bool CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2);
	
	static bool SphereAndBox(const SphereData& sphere, const BoxData& box);
	static bool SphereAndLineSegmrnt3D(const SphereData& sphere, const LineSegment3DData& lineSegment);
	static bool SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule);

	static bool BoardAndLineSegment3D(const BoardData& board, const LineSegment3DData& lineSegment);
};

