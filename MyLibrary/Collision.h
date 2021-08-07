#pragma once
#include"CollisionType.h"
#include"Values.h"

//�v�Z�Ɏg�����ƁA�v�Z�ɂ���ē���ꂽ���̍\���̕�����?
//�܂Ƃ߂��const�œn���Ȃ����A���p�҂��ǂ���͂���΂����̂��ς��ƌ��킩��ɂ���

namespace MelLib
{
	//�����蔻��(�̌v�Z)�N���X
	class Collision
	{

	private:
		static Value2<Vector3>CalcCapsuleLineSegmentPos(const CapsuleData& capsule);

	public:

#pragma region 2D

		/// <summary>
		/// �l�p�`���m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="rect1">�l�p�`</param>
		/// <param name="rect2">�l�p�`</param>
		/// <returns>�������Ă�����true</returns>
		static bool RectAndRect(const RectData& rect1, const RectData& rect2);

		/// <summary>
		/// �~���m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="circle1">�~</param>
		/// <param name="circle2">�~</param>
		/// <returns>�������Ă�����true</returns>
		static bool CircleAndCircle
		(
			const CircleData& circle1,
			const CircleData& circle2
		);


		static bool RectAndCircle(const RectData& rect, const CircleData& circle);

		/// <summary>
		/// �~��2D�̐������������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="circle">�~</param>
		/// <param name="lineSegment">2D�̐���</param>
		/// <returns>�������Ă�����true</returns>
		static bool CircleAndLineSegment2D
		(
			const CircleData& circle,
			CircleCalcResult* circleCalcResult,
			const LineSegment2DData& lineSegment,
			LineSegment2DCalcResult* lineSegmentCalcResult
		);

#pragma endregion


#pragma region 3D

		/// <summary>
		/// �����m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="sphere1">��</param>
		/// <param name="sphere2">��</param>
		/// <returns>�������Ă�����true</returns>
		static bool SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2);

		/// <summary>
		/// �����m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="box1">��</param>
		/// <param name="boxCalcResult1">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <param name="box2">��</param>
		/// <param name="boxCalcResult2">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <returns>�������Ă�����true</returns>
		static bool BoxAndBox(const BoxData& box1, BoxCalcResult* boxCalcResult1, const BoxData& box2, BoxCalcResult* boxCalcResult2);

		static bool LineSegment3DAndLineSegment3D
		(
			const LineSegment3DData& lineSegment1,
			LineSegment3DCalcResult* lineSegment1CalcResult,
			const LineSegment3DData& lineSegment2,
			LineSegment3DCalcResult* lineSegment2CalcResult
		);

		/// <summary>
		/// �J�v�Z�����m���������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="capsule1">�J�v�Z��</param>
		/// <param name="capsule2">�J�v�Z��</param>
		/// <returns>�������Ă�����true</returns>
		static bool CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2);

		/// <summary>
		/// ���Ɣ����������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="sphere">��</param>
		/// <param name="sphereCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <param name="box">��</param>
		/// <param name="boxCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <returns>�������Ă�����true</returns>
		static bool SphereAndBox(const SphereData& sphere, SphereCalcResult* sphereCalcResult, const BoxData& box, BoxCalcResult* boxCalcResult);

		static bool SphereAndLineSegmrnt3D(const SphereData& sphere, const LineSegment3DData& lineSegment);

		static bool SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule);

		/// <summary>
		/// ��3D�̐������������Ă��邩���m�F���܂��B
		/// </summary>
		/// <param name="board">��</param>
		/// <param name="boardCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <param name="lineSegment">3D�̐���</param>
		/// <param name="lineSegmentCalcResult">�v�Z���ʂ�����ϐ��̃|�C���^</param>
		/// <returns>�������Ă�����true</returns>
		static bool BoardAndLineSegment3D
		(
			const BoardData& board,
			BoardCalcResult* boardCalcResult,
			const LineSegment3DData& lineSegment,
			LineSegment3DCalcResult* lineSegmentCalcResult
		);
#pragma endregion
	};

}