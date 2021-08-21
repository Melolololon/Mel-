#pragma once
#include"Vector.h"
#include"Values.h"
namespace MelLib
{

	//CalcResult��CollisionResult�ɂ���?

	//�Փˊm�F�t���O
	struct CollisionDetectionFlag
	{
		bool sphere = true;
		bool box = true;
		bool ray = true;
		bool segment = true;
		bool plane = true;
		bool board = true;
		bool capsule = true;
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


	enum class BoxHitDirection
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


	struct SphereCalcResult
	{
		//���̂ǂ̕����ɂԂ��������Ƃ������
		BoxHitDirection boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
	};

	//��
	class SphereData
	{
	private:
		//���W
		Vector3 position;

		//���a
		float r = 0.0f;

		SphereCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		SphereCalcResult GetCalcResult() const { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetRadius(const float radius) { r = radius; }
	};


#pragma endregion

#pragma region AABB


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

#pragma region OBB

#pragma endregion

#pragma region ����

	//����
	class PlaneData
	{
	private:
		Vector3 normal;
		float distance = 0.0f;

	public:
		Vector3 GetNormal() { return normal; }
		float GetDistance() { return distance; }

		void SetNormal(const Vector3& normal) { this->normal = normal; }
		void SetDistance(const float dis) { distance = dis; }
	};
#pragma endregion

#pragma region ��

	struct BoardCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

	//��
	//�̔���B����]���́A0,0,1�̕����������܂��B
	class BoardData
	{
	private:
		Vector3 position;
		Vector2 size;
		Vector3 angle;
		BoardCalcResult result;

		Vector3 normal = Vector3(0, 0, -1);
		Vector3 leftDownPos;
		Vector3 leftUpPos;
		Vector3 rightUpPos;
		Vector3 rightDownPos;

	public:
		Vector3 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }
		Vector3 GetAngle()const { return angle; }
		BoardCalcResult GetCalcResult()const { return result; }

		Vector3 GetNormal()const { return normal; }
		//�����A����A�E���A�E��̏��Ŋi�[����Ă���
		Value4<Vector3>GetVertexPosition()const { Value4<Vector3>(leftDownPos, leftUpPos, rightDownPos, rightUpPos); }


		void SetPosition(const Vector3& pos) { position = pos; }
		void SetSize(const Vector2& size);
		void SetAngle(const Vector3& angle);
	};


#pragma endregion


#pragma region ����3D
	struct Segment3DCalcResult
	{
		Vector3 lineSegment3DHitPos;
		Vector3 boardHitPos;
	};

	//����
	class Segment3DData
	{
	private:
		Value2<Vector3> position;
		Vector3 angle = 0;

		//angle��K�����������W
		Value2<Vector3> rotatePosition;

		Segment3DCalcResult result;

	private:
		//angle�K���������p�x��rotatePosition�ɑ������֐�
		void CalcRotatePosition();

		
	public:
		Value2<Vector3> GetPosition()const { return position; }
		Value2<Vector3> GetRotatePosition()const { return rotatePosition; }
		Vector3 GetAngle()const { return angle; }
		Segment3DCalcResult GetCalcResult()const { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetAngle(const Vector3& angle);

	
	};

	

#pragma endregion

#pragma region ���C

	//��
	class RayData
	{
	private:
		Vector3 position;
		Vector3 direction;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetDirection()const { return direction; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetDirection(const Vector3& dir) { direction = dir; }
	};
#pragma endregion


#pragma region �J�v�Z��


	//�J�v�Z��
	class CapsuleData
	{
	private:
		Segment3DData segmentData;
		float r = 0.0f;

	public:
		Segment3DData GetSegment3DData()const { return segmentData; }
		float GetRadius()const { return r; }

		void SetSegment3DData(const Segment3DData& data) { segmentData = data; }
		void SetRadius(const float radius) { r = radius; }

	};

#pragma endregion


#pragma endregion

}