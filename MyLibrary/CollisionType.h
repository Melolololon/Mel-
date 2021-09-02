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
	class RectData
	{
	private:
		//�l�p�`�̍���
		Vector2 position;
		//�ӂ̒���
		Vector2 size;

	public:

		Vector2 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetSize(const Vector2& size) { this->size = size; }
	};
#pragma endregion

#pragma region �~

	//�~�̌v�Z����
	struct CircleCalcResult
	{
		Vector2 lineSegmentNearPosition;
	};

	//�~
	class CircleData
	{
	private:
		Vector2 position;
		float r = 0.0f;

		CircleCalcResult result;
	public:
		Vector2 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		CircleCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector2& pos) { position = pos; }
		void SetRadius(const float r) { this->r = r; }
		void SetCalcResult(const CircleCalcResult& result) { this->result = result; }
	};

#pragma endregion

#pragma region ��`
	class CircularSectorData
	{
	private:
		CircleData circleData;

		//����
		Vector2 direction;
		//�p�x(���v)
		float angle = 0.0f;

	public:
		CircleData GetCircleData()const { return circleData; }
		CircleData& GetRefCircleData() { return circleData; }
		Vector2 GetDirection()const { return direction; }
		float GetAngle()const { return angle; }

		void SetCircleData(const CircleData& data) { circleData = data; }
		void SetDirection(const Vector2 direction) { this->direction = direction; }
		void SetAngle(const float angle) { this->angle = angle; }
	};
#pragma endregion


#pragma region ����2D
	enum class SegmentHitPlace
	{
		LS_HIT_POSITION_NOT_HIT,//�Փ˂��ĂȂ�
		LS_HIT_POSITION_LE_START_END,//���̒[(�n�_�I�_)
		LS_HIT_POSITION_LE_LINE,//���̒[�ȊO
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

	class Segment2DData
	{
	private:
		//���W(�n�_�I�_)
		Value2<Vector2> position;

		Segment2DCalcResult result;
	public:

		Value2<Vector2> GetPosition()const { return position; }
		Segment2DCalcResult GetCalcResult()const { return result; }

		void SetPosition(const Value2<Vector2>& pos) { position = pos; }
		void SetCalcResult(const Segment2DCalcResult& result) { this->result = result; }
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
		void SetCalcResult(const SphereCalcResult& result) { this->result = result; }
	};


#pragma endregion

#pragma region AABB
	struct BoxCalcResult
	{

		//���肪�ǂ��ɂԂ�������
		BoxHitDirection boxHitDistance;
	};

	//��
	class BoxData
	{
	private:
		Vector3 position;
		Vector3 size;
		BoxCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetSize()const { return size; }
		BoxCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetSize(const Vector3& size) { this->size = size; }
		void SetCalcResult(const BoxCalcResult& result) { this->result = result; }
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
		Vector3 GetNormal()const { return normal; }
		float GetDistance()const { return distance; }

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

	private:
		void CalcRotateDirPosition();

	public:
		Vector3 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }
		Vector3 GetAngle()const { return angle; }
		BoardCalcResult GetCalcResult()const { return result; }

		Vector3 GetNormal()const { return normal; }
		//�����A����A�E���A�E��̏��Ŋi�[����Ă���
		Value4<Vector3>GetVertexPosition()const { return Value4<Vector3>(leftDownPos, leftUpPos, rightDownPos, rightUpPos); }


		void SetPosition(const Vector3& pos);
		void SetSize(const Vector2& size);
		void SetAngle(const Vector3& angle);
		void SetCalcResult(const BoardCalcResult& result) { this->result = result; }
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
		//��]������߂�ϐ�(0.0f��v1��A0.5f�Œ��S�A1.0f��v2�)
		float rotatePoint = 0.5f;

		Vector3 axisAngle;


		Vector3 axisXVector = MelLib::Vector3(-1, 0, 0);
		Vector3 axisYVector = MelLib::Vector3(0, -1, 0);
		Vector3 axisZVector = MelLib::Vector3(0, 0, -1);

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
		float GetRotatePoint()const { return rotatePoint; }
		Segment3DCalcResult GetCalcResult()const { return result; }


		void SetPosition(const Value2<Vector3>& pos);
		void SetAngle(const Vector3& angle);
		/// <summary>
		/// //��]������߂鐔�l(0.0f��v1��A0.5f�Œ��S�A1.0f��v2�)���Z�b�g���܂��B
		/// </summary>
		/// <param name="num">��]������߂鐔�l(0.0f��v1��A0.5f�Œ��S�A1.0f��v2�)</param>
		void SetRotatePoint(const float num);

		/// <summary>
		/// ��������]������̂Ɏg�p����ZXY�����̂̊p�x��ݒ肵�܂��B
		/// </summary>
		/// <param name="angle"></param>
		void SetAxisAngle(const Vector3& angle);

		void SetCalcResult(const Segment3DCalcResult& result) { this->result = result; }

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

		Segment3DData& GetRefSegment3DData() { return segmentData; }

		void SetSegment3DData(const Segment3DData& data) { segmentData = data; }
		void SetRadius(const float radius) { r = radius; }

	};

#pragma endregion


#pragma endregion

}