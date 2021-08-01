#pragma once
#include"Vector.h"

//����
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
	LS_HIT_POSITION_NOT_HIT,//�Փ˂��ĂȂ�
	LS_HIT_POSITION_LE_START_END,//���̒[(�n�_�I�_)
	LS_HIT_POSITION_LE_LINE,//��
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
//�l�p�`
struct RectData
{
	//�l�p�`�̍���
	Vector2 position;
	//�ӂ̒���
	Vector2 size;
};

//�~
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

//��
struct SphereData
{
	Vector3 position;

	//���a
	float r;

	//box�̂ǂ��ɂԂ�������
	BoxHitDirection boxHitDistance;
};

//��
struct BoxData
{
	Vector3 size;
	Vector3 position;

	//�����ǂ��ɂԂ�������
	BoxHitDirection boxHitDistance;
};


//����
struct PlaneData
{
	Vector3 normal;
	float distance;
};

//��
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

//����
struct LineSegment3DData
{
	Vector3 position[2];

	Vector3 hitPos;
};

//��
struct RayData 
{
	Vector3 pos;
	Vector3 direction;
};

//�J�v�Z��
struct CapsuleData
{
	SphereData sphereData[2];
};

#pragma endregion

