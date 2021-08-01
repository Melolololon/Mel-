#pragma once
#include"CollisionType.h"

//計算に使う式と、計算によって得られた情報の構造体分ける?
//まとめるとconstで渡せないし、利用者がどれ入力すればいいのかぱっと見わかりにくい

//当たり判定(の計算)クラス
class Collision
{
#pragma region 2D

	/// <summary>
	/// 四角形同士が当たっているかを確認します。
	/// </summary>
	/// <param name="rect1">四角形</param>
	/// <param name="rect2">四角形</param>
	/// <returns>当たっていたらtrue</returns>
	static bool RectAndRect(const RectData& rect1, const RectData& rect2);

	/// <summary>
	/// 円同士が当たっているかを確認します。
	/// </summary>
	/// <param name="circle1">円</param>
	/// <param name="circle2">円</param>
	/// <returns>当たっていたらtrue</returns>
	static bool CircleAndCircle
	(
		const CircleData& circle1,
		const CircleData& circle2
	);


	static bool RectAndCircle(const RectData& rect, const CircleData& circle);
	
	/// <summary>
	/// 円と2Dの線分が当たっているかを確認します。
	/// </summary>
	/// <param name="circle">円</param>
	/// <param name="lineSegment">2Dの線分</param>
	/// <returns>当たっていたらtrue</returns>
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
	/// 球同士が当たっているかを確認します。
	/// </summary>
	/// <param name="sphere1">球</param>
	/// <param name="sphere2">球</param>
	/// <returns>当たっていたらtrue</returns>
	static bool SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2);
	
	/// <summary>
	/// 箱同士が当たっているかを確認します。
	/// </summary>
	/// <param name="box1">箱</param>
	/// <param name="boxCalcResult1">計算結果を入れる変数のポインタ</param>
	/// <param name="box2">箱</param>
	/// <param name="boxCalcResult2">計算結果を入れる変数のポインタ</param>
	/// <returns>当たっていたらtrue</returns>
	static bool BoxAndBox(const BoxData& box1,BoxCalcResult* boxCalcResult1, const BoxData& box2, BoxCalcResult* boxCalcResult2);

	static bool LineSegment3DAndLineSegment3D(const LineSegment3DData& lineSegment1 , const LineSegment3DData& lineSegment2);
	static bool CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2);
	
	/// <summary>
	/// 球と箱が当たっているかを確認します。
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="sphereCalcResult">計算結果を入れる変数のポインタ</param>
	/// <param name="box">箱</param>
	/// <param name="boxCalcResult">計算結果を入れる変数のポインタ</param>
	/// <returns>当たっていたらtrue</returns>
	static bool SphereAndBox(const SphereData& sphere,SphereCalcResult* sphereCalcResult ,const BoxData& box,BoxCalcResult* boxCalcResult);
	
	static bool SphereAndLineSegmrnt3D(const SphereData& sphere, const LineSegment3DData& lineSegment);
	static bool SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule);

	static bool BoardAndLineSegment3D
	(
		const BoardData& board, 
		BoardCalcResult& boardCalcResult, 
		const LineSegment3DData& lineSegment,
		LineSegment3DCalcResult lineSegmentCalcResult
	);
#pragma endregion
};

