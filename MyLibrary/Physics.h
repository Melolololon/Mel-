#pragma once
#include"Vector.h"
#include"Values.h"

//�����W��float�ł��悳������������float�ɕς���

//�������Z�N���X
class Physics
{
public:

	/// <summary>
	/// �����W�������߂܂��B
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="hitPreviousVelocity">�ՓˑO</param>
	/// <param name="hitAfterVelocity">�Փˌ�</param>
	/// <returns></returns>
	static Vector3 CalcCoefficientOfRestitution
	(
		const Value2<Vector3>& position,
		const Value2<Vector3>& hitPreviousVelocity,
		const Value2<Vector3>& hitAfterVelocity
	);

	/// <summary>
	/// ���̂��Փ˂����Ƃ���velocity��Ԃ��܂��B
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">���݂̑��x</param>
	/// <param name="mass">�d��</param>
	/// <param name="e">�����W��(0����1)</param>
	/// <returns>�v�Z���ʁB</returns>
	static Value2<Vector3> CalcRepulsionVelocity
	(
		const Value2<Vector3>& position,
		const Value2<Vector3>& velocity,
		const Value2<float>& mass,
		const Value2<Vector3>& e
	);
};

