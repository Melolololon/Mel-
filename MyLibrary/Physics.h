#pragma once
#include"Vector.h"
#include"Values.h"
//�������Z�N���X
class Physics
{
public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="velocity"></param>
	/// <returns></returns>
	static float CalcCoefficientOfRestitution(const Vector3& hitPreviousVelocity,const Vector3& hitAfterVelocity);

	/// <summary>
	/// ���̂��Փ˂����Ƃ���velocity��Ԃ��܂��B
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">����velocity</param>
	/// <param name="mass">�d��</param>
	/// <param name="e">�����W��(0����1)</param>
	/// <returns>�v�Z����(1�̌��ʂ�v1�A2�̌��ʂ�v2�ɓ���)�B</returns>
	static Value2<Vector3> CalcRepulsionVelocity
	(
		const Value2<Vector3>& position,
		const Value2<Vector3>& velocity,
		const Value2<float>& mass,
		const Value2<Vector3>& e
	);
};

