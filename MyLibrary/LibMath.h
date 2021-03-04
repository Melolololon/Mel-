#pragma once
#include"Vector.h"
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
	/// num1��num2�̒l�̍������߁Atrue��false��Ԃ��܂�
	/// </summary>
	/// <param name="num1">�l1</param>
	/// <param name="num2">�l2</param>
	/// <param name="difference">��̒l</param>
	/// <param name="type">0 ��̒l�ȓ���true  1 ��̒l�ȏ��true</param>
	/// <returns></returns>
	static bool difference(float num1, float num2, float difference, int type);

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

	/// <summary>
	/// 2�̍��W�̋������擾���܂�
	/// </summary>
	/// <param name="pos1">���W1</param>
	/// <param name="pos2">���W2</param>
	/// <returns></returns>
	static float calcDistance3D(Vector3 pos1, Vector3 pos2);

	/// <summary>
	/// �ʂ̃I�u�W�F�N�g�ւ̃x�N�g�����擾���܂�(���K���͍s��)
	/// </summary>
	/// <param name="myPos"></param>
	/// <param name="otherPos"></param>
	/// <returns></returns>
	static Vector3 otherVector(Vector3 myPos, Vector3 otherPos);


	/// <summary>
	/// 2�̃x�N�g�����Ȃ��p�x�����߂܂�
	/// </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	static float twoVector2Angle(Vector2 v1, Vector2 v2);

	/// <summary>
	/// �x�N�g�����p�x�ɕϊ����܂�
	/// </summary>
	/// <param name="v"></param>
	/// <returns></returns>
	static float vecto2rToAngle(Vector2 v);

	/// <summary>
	/// �p�x��Vector2�ɕϊ����܂�
	/// </summary>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Vector2 angleToVector2(float angle);

	/// <summary>
	/// Vector2�̃x�N�g�����w�肵���p�x�ɉ����ĉ�]�����܂�
	/// </summary>
	/// <param name="v"></param>
	/// <param name="angle"></param>
	/// <returns></returns>
	static Vector2 rotateVector2(Vector2 v, float angle);
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
	static bool layAndPlaneCollision
	(
		Vector3 layStartPos,
		Vector3 layDirection,
		Vector3 normal,
		float planeDistance,
		float* distance,
		Vector3* crossPos
	);

	static bool layAndTryangleCollision
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
	static bool layAndSphereCollision
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

