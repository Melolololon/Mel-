#pragma once
#include"Vector.h"
#include"Values.h"

//反発係数floatでもよさそうだったらfloatに変える

//物理演算クラス
class Physics
{
public:
	
	//重力加速度(水星)
	static const float GRAVITATIONAL_ACCELERATION_MERCURY;
	//重力加速度(金星)
	static const float GRAVITATIONAL_ACCELERATION_VENUS;
	//重力加速度(地球)
	static const float GRAVITATIONAL_ACCELERATION_EARTH;
	//重力加速度(火星)
	static const float GRAVITATIONAL_ACCELERATION_MARS;
	//重力加速度(木星)
	static const float GRAVITATIONAL_ACCELERATION_JUPITER;
	//重力加速度(土星)
	static const float GRAVITATIONAL_ACCELERATION_SATURN;
	//重力加速度(天王星)
	static const float GRAVITATIONAL_ACCELERATION_URANUS;
	//重力加速度(海王星)
	static const float GRAVITATIONAL_ACCELERATION_NEPTUNE;
	//重力加速度(冥王星)
	static const float GRAVITATIONAL_ACCELERATION_PLUTO;
	//重力加速度(月)
	static const float GRAVITATIONAL_ACCELERATION_MOON;
		

	/// <summary>
	/// 反発係数を求めます。
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="hitPreviousVelocity">衝突前</param>
	/// <param name="hitAfterVelocity">衝突後</param>
	/// <returns></returns>
	static Vector3 CalcCoefficientOfRestitution
	(
		const Value2<Vector3>& position,
		const Value2<Vector3>& hitPreviousVelocity,
		const Value2<Vector3>& hitAfterVelocity
	);

	/// <summary>
	/// 物体が衝突したときのvelocityを返します。
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velocity">現在の速度</param>
	/// <param name="mass">重さ</param>
	/// <param name="e">反発係数(0から1)</param>
	/// <returns>計算結果。</returns>
	static Value2<Vector3> CalcRepulsionVelocity
	(
		const Value2<Vector3>& position,
		const Value2<Vector3>& velocity,
		const Value2<float>& mass,
		const Value2<Vector3>& e
	);

	
	//static Vector2 CalcSpringVelocity
	//(
	//	std::vector<Vector3>positions,
	//	const float gravitationalAcceleration,

	//)
};

