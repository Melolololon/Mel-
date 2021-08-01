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

#pragma region �l�p�`

//�l�p�`
struct RectData
{
	//�l�p�`�̍���
	Vector2 position;
	//�ӂ̒���
	Vector2 size;
};
#pragma endregion

#pragma region �~


//�~
struct CircleData
{
	Vector2 position;
	float r = 0.0f;

};

//�~�̌v�Z����
struct CircleCalcResult
{
	Vector2 lineSegmentNearPosition;
};
#pragma endregion

#pragma region ����2D
enum LineSegmentHitPlace
{
	LS_HIT_POSITION_NOT_HIT,//�Փ˂��ĂȂ�
	LS_HIT_POSITION_LE_START_END,//���̒[(�n�_�I�_)
	LS_HIT_POSITION_LE_LINE,//���̒[�ȊO
};


struct LineSegment2DData
{
	//���W(�n�_�I�_)
	Vector2 position[2];


};

struct LineSegment2DCalcResult
{
	//�������Ă���W
	Vector2 hitPos;
	//���������ꏊ
	LineSegmentHitPlace hitPlace;

	//�ŋߓ_
	Vector2 nearPos;
};


#pragma endregion


#pragma endregion

#pragma region 3D

#pragma region ��


//��
struct SphereData
{
	Vector3 position;

	//���a
	float r;

};

struct SphereCalcResult
{
	//���̂ǂ̕����ɂԂ��������Ƃ������
	BoxHitDirection boxHitDistance;
};

#pragma endregion

#pragma region ��


//��
struct BoxData
{
	Vector3 size;
	Vector3 position;

};

struct BoxCalcResult
{

	//���肪�ǂ��ɂԂ�������
	BoxHitDirection boxHitDistance;
};

#pragma endregion

#pragma region ����

//����
struct PlaneData
{
	Vector3 normal;
	float distance = 0.0f;
};
#pragma endregion

#pragma region ��


//��
struct BoardData
{
	Vector3 normal;
	Vector3 position;
	Vector3 leftDownPos;
	Vector3 leftUpPos;
	Vector3 rightUpPos;
	Vector3 rightDownPos;

};

struct BoardCalcResult
{
	Vector3 hitPos;
};

#pragma endregion


#pragma region ����3D

//����
struct LineSegment3DData
{
	Vector3 position[2];

};

struct LineSegment3DCalcResult
{
	Vector3 hitPos;
};

#pragma endregion

#pragma region ��

//��
struct RayData 
{
	Vector3 pos;
	Vector3 direction;
};
#pragma endregion


#pragma region �J�v�Z��


//�J�v�Z��
struct CapsuleData
{
	SphereData sphereData[2];
};

#pragma endregion


#pragma endregion

