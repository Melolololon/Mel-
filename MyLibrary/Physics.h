#pragma once
#include"Vector.h"
#include"Values.h"

//反発係数floatでもよさそうだったらfloatに変える

namespace melLib
{
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


		/// <summary>
		/// ばね定数を計算します。
		/// </summary>
		/// <param name="modulesOfRigidity">横弾性係数(N/mm2またはMPa)</param>
		/// <param name="linear">線径(mm)</param>
		/// <param name="turnsNum">有効巻き数</param>
		/// <param name="averageCoilDiameter">平均コイル径(mm)</param>
		/// <returns>ばね定数</returns>
		static float CalcSpringConstant
		(
			const float modulesOfRigidity,
			const float linear,
			const float turnsNum,
			const float	averageCoilDiameter
		)
		{
			return
				(modulesOfRigidity * (linear * linear * linear * linear))
				/ (8 * turnsNum * (averageCoilDiameter * averageCoilDiameter * averageCoilDiameter));
		}

		/// <summary>
		/// rootPosに物体をばねでつなげた時の速度を計算します。
		/// </summary>
		/// <param name="currentPos">現在の座標</param>
		/// <param name="currentVel">現在の速度</param>
		/// <param name="prePos">物体がつながっている場所の座標</param>
		/// <param name="naturalDis">自然長</param>
		/*/// <param name="maxDis">最大距離</param>*/
		/// <param name="mass">物体の重さ</param>
		/// <param name="gravitationalAcceleration">重力加速度(このクラスで定義されている定数または自分で考えた値)</param>
		/// <param name="springConstant">ばね定数</param>
		/// <param name="viscousDragCoefficient">粘性抵抗係数</param>
		/// <returns></returns>
		static Vector3 CalcSpringVelocity
		(
			const Vector3& currentPos,
			const Vector3& currentVel,
			const Vector3* prePos,
			const Vector3* nextPos,
			const float naturalDis,
			/*const float maxDis,*/
			const float mass,
			const float gravitationalAcceleration,
			const float springConstant,
			const float viscousDragCoefficient
		);


	};

}