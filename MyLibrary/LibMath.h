#pragma once
#include"Vector.h"
#include"Matrix.h"
#include"Quaternion.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"Library.h"
//Game��Math�N���X�����������疼�O��Math�ɂ���ƃG���[���o��
//Math�ӊO���ƃG���[�o�Ȃ������̂ŁAMath�Ƃ������O���̂������Ȃ��̂�������Ȃ�
//�G���[�o�Ȃ����Ƃ�����
//include����Ɗm���ɃG���[���o��Binclude���Ȃ��Əo�Ȃ�
//lib(�X�^�e�B�b�N���C�u�����̃v���W�F�N�g)�ɍ��Ȃ���΃G���[�o�Ȃ�?
class LibMath
{
private:

public:

	/// <summary>
	/// num1��num2�̒l�̍������߁A�� <= ��̒l��������true��Ԃ��܂�
	/// </summary>
	/// <param name="num1">�l1</param>
	/// <param name="num2">�l2</param>
	/// <param name="difference">��̒l</param>
	/// <returns></returns>
	static bool difference(const float& num1, const float& num2, const float& difference);

	/// <summary>
		/// �x���@�̊p�x�����W�A���Ɂ@���W�A����x���@�̊p�x��
		/// </summary>
		/// <param name="pattern">0 �x���@�̊p�x�����W�A����   1 ���W�A����x���@�̊p�x��</param>
		/// <param name="angle">�p�x</param>
	static float angleConversion(int pattern, float angle);


	/// <summary>
	/// �~�������擾���܂�
	/// </summary>
	/// <returns></returns>
	static float getFloatPI();

	/// <summary>
	/// �~�������擾���܂�
	/// </summary>
	/// <returns></returns>
	static double getDoublePI();

#pragma region �x�N�g��


#pragma region vector2

	/// <summary>
	/// ���E������s���܂��B�_���x�N�g�����E�̏ꍇ��1�A���̏ꍇ��-1�A�x�N�g����̏ꍇ��0��Ԃ��܂��B
	/// </summary>
	/// <param name="vector"></param>
	/// <param name="point"></param>
	/// <returns></returns>
	static char pointLeftRightCheck(const Vector2& vector, const Vector2& point);

	/// <summary>
	/// 2�̃x�N�g�����Ȃ��p�x�����߂܂�
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static float twoVector2Angle(const Vector2& v1, const Vector2& v2);

	/// <summary>
	/// �x�N�g�����p�x�ɕϊ����܂�
	/// </summary>
	/// <param name="v"></param>
	/// <param name="v3">3�������W�n���ǂ���</param>
	/// <returns></returns>
	static float vecto2ToAngle(const Vector2& v,const bool& v3 );

	/// <summary>
	/// �p�x��Vector2�ɕϊ����܂�
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="v3">3�������W�n���ǂ���</param>
	/// <returns></returns>
	static Vector2 angleToVector2(const float& angle, const bool& v3);

	/// <summary>
	/// Vector2�̃x�N�g�����w�肵���p�x�ɉ����ĉ�]�����܂�
	/// </summary>
	/// <param name="v"></param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Vector2 rotateVector2(const Vector2& v, const float& angle);
#pragma endregion

#pragma region Vector3

	/// <summary>
	/// 2�̍��W�̋������擾���܂�
	/// </summary>
	/// <param name="pos1">���W1</param>
	/// <param name="pos2">���W2</param>
	/// <returns></returns>
	static float calcDistance3D(Vector3 pos1, Vector3 pos2);

	/// <summary>
	/// vex1����vec2�̃x�N�g�����擾���܂�(���K���͍s��)
	/// </summary>
	/// <param name="myPos"></param>
	/// <param name="otherPos"></param>
	/// <returns></returns>
	static Vector3 otherVector(const Vector3& vec1, const Vector3& vec2);

	/// <summary>
	/// �x�N�g������]�����܂�
	/// </summary>
	/// <param name="rotateV"></param>
	/// <param name="vec">���̃x�N�g��</param>
	/// <param name="angle"></param>
	/// <returns></returns>
	Vector3 rotateVector3(const Vector3& rotateV, const Vector3& vec ,const float& angle);
#pragma endregion


#pragma endregion

#pragma region �����蔻��

#pragma region 2D

	/// <summary>
	/// �l�p�̓����蔻��ł�
	/// </summary>
	/// <param name="start1">1�ڂ̍�����W</param>
	/// <param name="end1">1�ڂ̉E������W</param>
	/// <param name="start2">1�ڂ̍�����W</param>
	/// <param name="end2">1�ڂ̉E�����W</param>
	/// <returns>�����������ǂ���</returns>
	static bool rectCollision(Vector2 start1, Vector2 end1, Vector2 start2, Vector2 end2);

	/// <summary>
	/// �~�̓����蔻��ł�
	/// </summary>
	/// <param name="centerPosition1">1�ڂ̍��W</param>
	/// <param name="r1">1�ڂ̔��a</param>
	/// <param name="centerPosition2">1�ڂ̍��W</param>
	/// <param name="r2">1�ڂ̉~</param>
	/// <returns>�����������ǂ���</returns>
	static bool circleCollision(Vector2 centerPosition1, float r1, Vector2 centerPosition2, float r2);

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
	static bool sphereCollision(Vector3 centerPosition1, float r1, Vector3 centerPosition2, float r2);

	/// <summary>
	/// ���ƕ��ʂ̍��W
	/// </summary>
	/// <param name="spherePos"></param>
	/// <param name="r"></param>
	/// <param name="normal">���ʂ̖@���x�N�g��</param>
	/// <param name="planeDist">���ʂ̌��_���畽�ʂ̋���(�}�C�i�X�\)</param>
	/// <param name="hitPos"></param>
	/// <returns></returns>
	static bool sphereAndPlaneCollision
	(
		Vector3 spherePos,
		float r,
		Vector3 normal,
		float planeDist,
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
	static bool sphereAndTryangleCorrision
	(
		Vector3 spherePos,
		float r,
		Vector3 triPos1,
		Vector3 triPos2,
		Vector3 triPos3,
		Vector3 normal,
		Vector3* hitPos
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
	static bool lineSegmentAndBoardCollision
	(
		Vector3 linePos1,
		Vector3 linePos2,
		Vector3 normal,
		Vector3 pointPos,
		std::vector<Vector3>vertexPoint,
		Vector3* crossPosition
	);

	/// <summary>
	/// �������ƕ��ʂ�
	/// </summary>
	/// <param name="layStartPos"></param>
	/// <param name="layDirection"></param>
	/// <param name="normal"></param>
	/// <param name="planeDist"></param>
	/// <param name="distance"></param>
	/// <param name="crossPos"></param>
	/// <returns></returns>
	static bool rayAndPlaneCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 normal,
		float planeDistance,
		float* distance,
		Vector3* crossPos
	);

	static bool rayAndTryangleCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 triPos1,
		Vector3 triPos2,
		Vector3 triPos3,
		Vector3 normal,
		float* distance,
		Vector3* crossPos
	);

	/// <summary>
	/// �������Ƌ��̓����蔻��
	/// </summary>
	static bool rayAndSphereCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 spherePos,
		float r,
		float* distance,
		Vector3* crossPos
	);
#pragma endregion

#pragma endregion


};

