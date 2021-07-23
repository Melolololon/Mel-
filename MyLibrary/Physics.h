#pragma once
#include"Vector.h"
#include"Values.h"
//物理演算クラス
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
	/// 物体が衝突したときのvelocityを返します。
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velocity">現在velocity</param>
	/// <param name="mass">重さ</param>
	/// <param name="e">反発係数(0から1)</param>
	/// <returns>計算結果(1の結果はv1、2の結果はv2に入る)。</returns>
	static Value2<Vector3> CalcRepulsionVelocity
	(
		const Value2<Vector3>& position,
		const Value2<Vector3>& velocity,
		const Value2<float>& mass,
		const Value2<Vector3>& e
	);
};

