#pragma once
#include"Vector.h"

//îªíË
struct CollisionFlag
{
	bool sphere = false;
	bool box = false;
	bool ray = false;
	bool lineSegment = false;
	bool plane = false;
	bool board = false;
};

enum CollisionType 
{
	COLLISION_RECT,
	COLLISION_CIRCLE,
	COLLISION_SPHERE,
	COLLISION_BOX,
	COLLISION_PLANE,
	COLLISION_BOARD,
	COLLISION_LINESEGMENT,
	COLLISION_RAY,

};

enum BoxHitDirection
{
	BOX_HIT_DIRECTION_NO_HIT,
	BOX_HIT_DIRECTION_UP,
	BOX_HIT_DIRECTION_DOWN,
	BOX_HIT_DIRECTION_LEFT,
	BOX_HIT_DIRECTION_RIGHT,
	BOX_HIT_DIRECTION_FRONT,
	BOX_HIT_DIRECTION_BACK,
};

#pragma region 2D

struct RectData
{
	//éläpå`ÇÃç∂è„
	Vector2 position;
	//ï”ÇÃí∑Ç≥
	Vector2 size;
};

struct CircleData
{
	Vector2 position;
	float r;
};

#pragma endregion

#pragma region 3D

struct SphereData
{
	Vector3 position;

	//îºåa
	float r;

	//boxÇÃÇ«Ç±Ç…Ç‘Ç¬Ç©Ç¡ÇΩÇ©
	BoxHitDirection boxHitDistance;
};

struct BoxData
{
	Vector3 size;
	Vector3 position;

	//ãÖÇ™Ç«Ç±Ç…Ç‘Ç¬Ç©Ç¡ÇΩÇ©
	BoxHitDirection boxHitDistance;
};

struct PlaneData
{
	Vector3 normal;
	float distance;
};

struct BoardData
{
	Vector3 normal;
	Vector3 position;
	Vector3 leftDownPos;
	Vector3 leftUpPos;
	Vector3 rightUpPos;
	Vector3 rightDownPos;

	Vector3 hitPos;
};

struct LineSegmentData
{
	Vector3 position[2];

	Vector3 hitPos;
};

struct RayData 
{
	Vector3 pos;
	Vector3 direction;
};



#pragma endregion

