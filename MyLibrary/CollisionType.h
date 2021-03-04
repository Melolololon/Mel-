#pragma once
#include"Vector.h"

//����
struct CollisionFlag
{
	bool sphere;
	bool ray;
	bool lineSegment;
	bool plane;
	bool board;
};

enum CollisionType 
{
	COLLISION_RECT,
	COLLISION_CIRCLE,
	COLLISION_SPHERE,
	COLLISION_PLANE,
	COLLISION_BOARD,
	COLLISION_LINESEGMENT,
	COLLISION_RAY,

};

#pragma region 2D

struct RectData
{
	//�l�p�`�̍���
	Vector2 position;
	//�ӂ̒���
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

	//���a
	float r;
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

