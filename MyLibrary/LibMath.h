#pragma once
#include"Vector.h"
#include"Matrix.h"
#include"Quaternion.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"Library.h"
#include"CollisionType.h"
//Game��Math�N���X�����������疼�O��Math�ɂ���ƃG���[���o��
//Math�ӊO���ƃG���[�o�Ȃ������̂ŁAMath�Ƃ������O���̂������Ȃ��̂�������Ȃ�
//�G���[�o�Ȃ����Ƃ�����
//include����Ɗm���ɃG���[���o��Binclude���Ȃ��Əo�Ȃ�
//lib(�X�^�e�B�b�N���C�u�����̃v���W�F�N�g)�ɍ��Ȃ���΃G���[�o�Ȃ�?

namespace MelLib
{

	//A�X�^�[�̃m�[�h
	struct AStarNode
	{
		//���W
		Vector2 position = 0;

		//�R�X�g
		UINT cost = 1;

		//�v�Z����(�X�e�b�v + ���� + �R�X�g)
		UINT calcNum = UINT_MAX;


		//�ȉ��T���p�ϐ�

		//�z��̃C���f�b�N�X
		int indexX = INT_MAX;
		int indexY = INT_MAX;

		//close�z��̃C���f�b�N�X
		int closeIndex = INT_MAX;

		//�X�^�[�g����̋���
		int stepNum = 0;

		AStarNode* previousNode = nullptr;

		bool openFlag = false;
		bool closeFlag = false;

		//�i�s�s�\�I�u�W�F�N�g�Əd�Ȃ��Ă���m�[�h
		bool hitObjectNode = false;

	};

	class LibMath
	{
	private:
		LibMath() {}
		~LibMath() {}
	public:
		//���x�ł�������A�v���C���[��G�̑傫�����l�����Čo�H���v�Z�ł���悤�ɂ���

#pragma region �ŒZ�o�H

	/// <summary>
	/// ��������Ƀm�[�h�̍��W���Z�b�g���܂��B
	/// </summary>
	/// <param name="leftUpPos">������W</param>
	/// <param name="rightDownPos">�E�����W</param>
	/// <param name="nodeNumX">��������</param>
	/// <param name="nodeNumY">�c������</param>
	/// <param name="nodes">�m�[�h��vector(size��0�ł悢)</param>
	/// <param name="upPlus">��������v���X���ǂ���</param>
		static void SetAStarNodePosition
		(
			const Vector2& leftUpPos,
			const Vector2& rightDownPos,
			const UINT& nodeNumX,
			const UINT& nodeNumY,
			std::vector< std::vector<AStarNode>>& nodes,
			const bool upPlus
		);

		//���̊֐��ŃR�X�g��1����Ȃ��I�u�W�F�N�g�Ƃ��̃R�X�g��n���悤�ɂ���?
		/// <summary>
		/// �m�[�h���i�s�s�\�I�u�W�F�N�g�Ƀq�b�g���Ă邩���m�F���܂��B
		/// </summary>
		/// <param name="hitObjectsPos">��Q���̍��W��vector</param>
		/// <param name="hitObjectsSize">��Q���̃T�C�Y��vector</param>
		/// <param name="nodes">�m�[�h��vector(SetAStarNodePosition�ɓn������̔z��)</param>
		static void SetAStarNodeHitObjectNodeFlag
		(
			const std::vector<Vector2>& hitObjectsPos,
			const std::vector<Vector2>& hitObjectsSize,
			std::vector< std::vector<AStarNode>>& nodes
		);

		/// <summary>
		/// �n���ꂽ�f�[�^�����ƂɍŒZ�o�H�����߂܂��B
		/// </summary>
		/// <param name="startPos">�X�^�[�g�n�_�̍��W</param>
		/// <param name="endPos">�S�[���n�_�̍��W</param>
		/// <param name="nodes">�m�[�h��vector(SetAStarNodeHitObjectNodeFlag�ɓn������̔z��)</param>
		/// <param name="routeVector">�S�[���܂ł̃��[�g���i�[����vector(size��0�ł悢)</param>
		/// <returns>�T���������������ǂ���</returns>
		static bool GetAStarCalcResult
		(
			const Vector2& startPos,
			const Vector2& endPos,
			std::vector< std::vector<AStarNode>>& nodes,
			std::vector<Vector2>& routeVector
		);

#pragma endregion


		/// <summary>
		/// �K��
		/// </summary>
		/// <param name="num"></param>
		/// <returns></returns>
		static int Factorial(const int num);

		/// <summary>
		/// �ݏ�
		/// </summary>
		/// <param name="num"></param>
		/// <param name="index"></param>
		/// <returns></returns>
		static int Pow(const float num, const int index);


		/// <summary>
		/// num1��num2�̒l�̍������߁A������̒l��菬����������true��Ԃ��܂�
		/// </summary>
		/// <param name="num1">�l1</param>
		/// <param name="num2">�l2</param>
		/// <param name="difference">��̒l</param>
		/// <returns></returns>
		static bool Difference(const float num1, const float num2, const float difference);

		/// <summary>
		/// angle1��angle2�̒l�̍������߁A������̒l��菬����������true��Ԃ��܂��B
		/// </summary>
		/// <param name="angle1">�p�x</param>
		/// <param name="angle2">�p�x</param>
		/// <param name="difference"></param>
		/// <returns></returns>
		static bool AngleDifference(const float angle1, const float angle2, const float difference);

		/// <summary>
			/// �x���@�̊p�x�����W�A���Ɂ@���W�A����x���@�̊p�x��
			/// </summary>
			/// <param name="pattern">0 �x���@�̊p�x�����W�A����   1 ���W�A����x���@�̊p�x��</param>
			/// <param name="angle">�p�x</param>
		static float AngleConversion(int pattern, float angle);


		/// <summary>
		/// �~�������擾���܂�
		/// </summary>
		/// <returns></returns>
		static float GetFloatPI();

		/// <summary>
		/// �~�������擾���܂�
		/// </summary>
		/// <returns></returns>
		static double GetDoublePI();



#pragma region �x�N�g��


#pragma region vector2


		/// <summary>
		/// 2�̍��W�̋������擾���܂�
		/// </summary>
		/// <param name="pos1">���W1</param>
		/// <param name="pos2">���W2</param>
		/// <returns></returns>
		static float CalcDistance2D(const Vector2& pos1, const Vector2& pos2);


		/// <summary>
		/// ���E������s���܂��B�_���x�N�g�����E�̏ꍇ��1�A���̏ꍇ��-1�A�x�N�g����̏ꍇ��0��Ԃ��܂��B
		/// </summary>
		/// <param name="vector"></param>
		/// <param name="point"></param>
		/// <returns></returns>
		static char PointLeftRightCheck(const Vector2& vector, const Vector2& point);

		/// <summary>
		/// 2�̃x�N�g�����Ȃ��p�x�����߂܂�
		/// </summary>
		/// <param name="v1"></param>
		/// <param name="v2"></param>
		/// <returns></returns>
		static float TwoVector2Angle(const Vector2& v1, const Vector2& v2);

		/// <summary>
		/// �x�N�g�����p�x�ɕϊ����܂�
		/// </summary>
		/// <param name="v"></param>
		/// <param name="v3">3�������W�n���ǂ���</param>
		/// <returns></returns>
		static float Vecto2ToAngle(const Vector2& v, const bool& v3);

		/// <summary>
		/// �p�x��Vector2�ɕϊ����܂�
		/// </summary>
		/// <param name="angle"></param>
		/// <param name="v3">3�������W�n���ǂ���</param>
		/// <returns></returns>
		static Vector2 AngleToVector2(const float& angle, const bool& v3);

		/// <summary>
		/// Vector2�̃x�N�g�����w�肵���p�x�ɉ����ĉ�]�����܂�
		/// </summary>
		/// <param name="v"></param>
		/// <param name="angle"></param>
		/// <returns></returns>
		static Vector2 RotateVector2(const Vector2& v, const float& angle);
#pragma endregion

#pragma region Vector3

		/// <summary>
		/// �����̍��W3����@���x�N�g�������߂܂��Bpos1����pos3�����ԂɒH�����Ƃ��ɁA�E����W�n�̏ꍇ�͉E���A������W�n�̏ꍇ�͍����ɂȂ�悤�ɂ��Ă��������B
		/// </summary>
		/// <param name="pos1"></param>
		/// <param name="pos2"></param>
		/// <param name="pos3"></param>
		/// <returns></returns>
		static Vector3 CalcNormal(const Vector3& pos1, const Vector3& pos2, const Vector3& pos3);

		/// <summary>
		/// 2�̍��W�̋������擾���܂�
		/// </summary>
		/// <param name="pos1">���W1</param>
		/// <param name="pos2">���W2</param>
		/// <returns></returns>
		static float CalcDistance3D(const Vector3& pos1, const Vector3& pos2);

		/// <summary>
		/// vex1����vec2�̃x�N�g�����擾���܂�(���K���͍s��)
		/// </summary>
		/// <param name="myPos"></param>
		/// <param name="otherPos"></param>
		/// <returns></returns>
		static Vector3 OtherVector(const Vector3& vec1, const Vector3& vec2);

		/// <summary>
		/// �x�N�g������]�����܂�
		/// </summary>
		/// <param name="rotateV"></param>
		/// <param name="vec">���̃x�N�g��</param>
		/// <param name="angle"></param>
		/// <returns></returns>
		static Vector3 RotateVector3(const Vector3& rotateV, const Vector3& vec, const float& angle);


		/// <summary>
		/// ���W���w�肵���x�N�g��������distance���ړ�������Vector3�^�̒l��Ԃ��܂��B
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="vector"></param>
		/// <param name="distance"></param>
		/// <returns></returns>
		static Vector3 FloatDistanceMoveVector3(const Vector3& pos, const Vector3& vector, const float distance);
#pragma endregion


#pragma endregion

#pragma region �����蔻��

#pragma region 2D

		/// <summary>
		/// �l�p�̓����蔻��ł��B���W������̏ꍇ�̂ݐ������v�Z�ł��܂��B
		/// </summary>
		/// <param name="start1">1�ڂ̍�����W</param>
		/// <param name="size1">1�ڂ̃T�C�Y</param>
		/// <param name="start2">2�ڂ̍�����W</param>
		/// <param name="size2">2�ڂ̃T�C�Y</param>
		/// <returns>�����������ǂ���</returns>
		static bool RectAndRectCollision
		(
			const Vector2& pos1,
			const Vector2& size1,
			const Vector2& pos2,
			const Vector2& size2
		);

		/// <summary>
		/// �~�̓����蔻��ł��B
		/// </summary>
		/// <param name="centerPosition1">1�ڂ̍��W</param>
		/// <param name="r1">1�ڂ̔��a</param>
		/// <param name="centerPosition2">1�ڂ̍��W</param>
		/// <param name="r2">1�ڂ̉~</param>
		/// <returns>�����������ǂ���</returns>
		static bool CircleAndCircleCollision
		(
			const Vector2& centerPosition1,
			const float r1,
			const Vector2& centerPosition2,
			const float r2
		);



		/// <summary>
		/// �~�Ɛ����̔���
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="linePos1"></param>
		/// <param name="linePos2"></param>
		/// <param name="nearPos">�~�Ƃ̍ŋߓ_</param>
		/// <param name="lsHit">���̂ǂ��ɓ���������</param>
		/// <returns></returns>
		static bool CircleAndLineSegmentCollision
		(
			const Vector2& spherePos,
			const float r,
			const Vector2& linePos1,
			const Vector2& linePos2,
			Vector2* nearPos,
			LineSegmentHitPlace* isHit
		);

#pragma endregion

#pragma region 3D
		/// <summary>
		/// �����m�̓����蔻��
		/// </summary>
		/// <param name="centerPosition1">1�ڂ̍��W</param>
		/// <param name="r1">1�ڂ̔��a</param>
		/// <param name="centerPosition2">1�ڂ̍��W</param>
		/// <param name="r2">1�ڂ̉~</param>
		/// <returns>�����������ǂ���</returns>
		static bool SphereAndSphereCollision
		(
			const Vector3& centerPosition1,
			const float r1,
			const Vector3& centerPosition2,
			const float r2
		);

		/// <summary>
		/// ���ƕ��ʂ̍��W
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="normal">���ʂ̖@���x�N�g��</param>
		/// <param name="planeDist">���ʂ̌��_���畽�ʂ̋���(�}�C�i�X�\)</param>
		/// <param name="hitPos"></param>
		/// <returns></returns>
		static bool SphereAndPlaneCollision
		(
			const Vector3& spherePos,
			const float r,
			const Vector3& normal,
			const float planeDist,
			Vector3* hitPos
		);

		/// <summary>
		/// ���ƕ���ȎO�p�`�̔���
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="triPos1"></param>
		/// <param name="triPos2"></param>
		/// <param name="triPos3"></param>
		/// <param name="normal"></param>
		/// <param name="hitPos"></param>
		/// <returns></returns>
		static bool SphereAndTryangleCollision
		(
			const Vector3& spherePos,
			const float r,
			const Vector3& triPos1,
			const Vector3& triPos2,
			const Vector3& triPos3,
			const Vector3& normal,
			Vector3* hitPos
		);

		static bool SphereAndLineSegmentCollision
		(
			const Vector3& spherePos,
			const float r,
			const Vector3& linePos1,
			const Vector3& linePos2
		);

		/// <summary>
		/// ���Ɨ�����(AABB)�̔���
		/// </summary>
		/// <param name="spherePos"></param>
		/// <param name="r"></param>
		/// <param name="minPos"></param>
		/// <param name="maxPos"></param>
		/// <param name="direction">���́A���̂ǂ��ɓ����������B���͋��������̂ǂ��ɂԂ���������Ԃ�</param>
		/// <returns></returns>
		static bool SphereAndBoxCollision
		(
			const Vector3& spherePos,
			const float& r,
			const Vector3& centerPos,
			const Vector3& size,
			BoxHitDirection* direction
		);

		/// <summary>
		/// ������(AABB)���m�̔���
		/// </summary>
		/// <param name="centerPos1"></param>
		/// <param name="size1"></param>
		/// <param name="centerPos2"></param>
		/// <param name="size2"></param>
		/// <param name="direction1">�����̂ǂ����Ԃ���������Ԃ�</param>
		/// <param name="direction2">�����̂ǂ����Ԃ���������Ԃ�</param>
		/// <returns></returns>
		static bool BoxAndBoxCollision
		(
			const Vector3& centerPos1,
			const Vector3& size1,
			const Vector3& centerPos2,
			const Vector3& size2,
			BoxHitDirection* direction1,
			BoxHitDirection* direction2
		);

		/// <summary>
		/// �����Ɣ̓����蔻��
		/// </summary>
		/// <param name="linePos1">��1</param>
		/// <param name="linePos2">��2</param>
		/// <param name="normal">�̖@���x�N�g��</param>
		/// <param name="pointPos">�̍��W</param>
		/// <param name="vertexPoint">���_�̍��W4��(��]�ȂǓK����)(��]�O�̍����A����A�E���A�E��̏��Ŕz��ɓ����)</param>
		/// <param name="crossPos">���ƃ|���S�����Ԃ������ꏊ</param>
		/// /// <returns></returns>
		static bool LineSegmentAndBoardCollision
		(
			const Vector3& linePos1,
			const Vector3& linePos2,
			const Vector3& normal,
			const Vector3& pointPos,
			const std::vector<Vector3>& vertexPoint,
			Vector3* crossPosition
		);

		/// <summary>
		/// ������(���C)�ƕ��ʂ̔���
		/// </summary>
		/// <param name="layStartPos"></param>
		/// <param name="layDirection"></param>
		/// <param name="normal"></param>
		/// <param name="planeDist"></param>
		/// <param name="distance"></param>
		/// <param name="crossPos"></param>
		/// <returns></returns>
		static bool RayAndPlaneCollision
		(
			const Vector3& layStartPos,
			const Vector3& layDirection,
			const Vector3& normal,
			const float planeDistance,
			float* distance,
			Vector3* crossPos
		);

		/// <summary>
		/// ������(���C)�ƕ���ȎO�p�`�̔���B
		/// </summary>
		/// <param name="layStartPos"></param>
		/// <param name="layDirection"></param>
		/// <param name="triPos1"></param>
		/// <param name="triPos2"></param>
		/// <param name="triPos3"></param>
		/// <param name="normal"></param>
		/// <param name="distance"></param>
		/// <param name="crossPos"></param>
		/// <returns></returns>
		static bool RayAndTryangleCollision
		(
			const Vector3& layStartPos,
			const Vector3& layDirection,
			const Vector3& triPos1,
			const Vector3& triPos2,
			const Vector3& triPos3,
			const Vector3& normal,
			float* distance,
			Vector3* crossPos
		);

		/// <summary>
		/// �������Ƌ��̓����蔻��
		/// </summary>
		static bool RayAndSphereCollision
		(
			const Vector3& layStartPos,
			const Vector3& layDirection,
			const Vector3& spherePos,
			const float r,
			float* distance,
			Vector3* crossPos
		);


#pragma endregion

#pragma endregion


	};

}