#pragma once
#include"Vector.h"

//判定
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

enum LineSegmentHitPlace
{
	LS_HIT_POSITION_NOT_HIT,//衝突してない
	LS_HIT_POSITION_LE_START_END,//線の端(始点終点)
	LS_HIT_POSITION_LE_LINE,//線
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
//四角形
struct RectData
{
	//四角形の左上
	Vector2 position;
	//辺の長さ
	Vector2 size;
};

//円
struct CircleData
{
	Vector2 position;
	float r;

	Vector2 lineSegmentNearPosition;
};

struct LineSegment2DData
{
	Vector2 position[2];
	Vector2 hitPos;

	LineSegmentHitPlace hitPlace;
};
#pragma endregion

#pragma region 3D

//球
struct SphereData
{
	Vector3 position;

	//半径
	float r;

	//boxのどこにぶつかったか
	BoxHitDirection boxHitDistance;
};

//箱
struct BoxData
{
	Vector3 size;
	Vector3 position;

	//球がどこにぶつかったか
	BoxHitDirection boxHitDistance;
};


//平面
struct PlaneData
{
	Vector3 normal;
	float distance;
};

//板
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

//線分
struct LineSegment3DData
{
	Vector3 position[2];

	Vector3 hitPos;
};

//線
struct RayData 
{
	Vector3 pos;
	Vector3 direction;
};

//カプセル
struct CapsuleData
{
	SphereData sphereData[2];
};

#pragma endregion

