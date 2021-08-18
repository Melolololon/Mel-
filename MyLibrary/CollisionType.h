#pragma once
#include"Vector.h"
#include"Values.h"
namespace MelLib
{

	//CalcResult��CollisionResult�ɂ���?

	//�Փˊm�F�t���O
	struct CollisionDetectionFlag
	{
		bool sphere = false;
		bool box = false;
		bool ray = false;
		bool segment = false;
		bool plane = false;
		bool board = false;
		bool capsule = false;
	};

	//2D�̌`��̗�
	enum class ShapeType2D
	{
		RECT,
		CIRCLE,
		SEGMENT,
	};

	//3D�̌`��̗�
	enum class ShapeType3D
	{
		SPHERE,
		BOX,
		PLANE,
		BOARD,
		SEGMENT,
		RAY,
		CAPSULE,
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

	//�f�[�^�ƃ��U���g�܂Ƃ߂��\���̗p�ӂ���?�������T�C�Y����̖ʓ|����

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
	enum SegmentHitPlace
	{
		LS_HIT_POSITION_NOT_HIT,//�Փ˂��ĂȂ�
		LS_HIT_POSITION_LE_START_END,//���̒[(�n�_�I�_)
		LS_HIT_POSITION_LE_LINE,//���̒[�ȊO
	};


	struct Segment2DData
	{
		//���W(�n�_�I�_)
		Vector2 position[2];


	};

	struct Segment2DCalcResult
	{
		//�������Ă���W
		Vector2 hitPos;
		//���������ꏊ
		SegmentHitPlace hitPlace;

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
		float r = 0.0f;

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
		Vector3 lineSegment3DHitPos;
	};

#pragma endregion


#pragma region ����3D

	//����
	struct Segment3DData
	{
		Vector3 position[2];
		Vector3 angle = 0;

		

		//����v�Z���t���[��1��ɂ���CalcResult�ɓ����悤�ɂ���?
		//�Z�b�g���Ă����擾�������ꍇ���邩������Ȃ����炱�̂܂܂ɂ���?
		/// <summary>
		/// �����̒��S�����angle����]���������[�̍��W��Ԃ��܂��B
		/// </summary>
		/// <returns></returns>
		Value2<Vector3>GetRotatePosition()const;

		/// <summary>
		/// 2�_�Ԃ̒��S���W���擾���܂��B
		/// </summary>
		/// <returns></returns>
		Vector3 GetCenterPosition ()const;

		/// <summary>
		/// 2�_�Ԃ̋��������߂܂��B
		/// </summary>
		/// <returns></returns>
		float GetPositionDistance()const;
	};

	struct Segment3DCalcResult
	{
		Vector3 lineSegment3DHitPos;
		Vector3 boardHitPos;
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
		Segment3DData segmentData;
		
		float r = 0.0f;
	};

#pragma endregion


#pragma endregion

}