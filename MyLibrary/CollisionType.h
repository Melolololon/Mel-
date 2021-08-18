#pragma once
#include"Vector.h"
#include"Values.h"
namespace MelLib
{

	//CalcResultをCollisionResultにする?

	//衝突確認フラグ
	struct CollisionDetectionFlag
	{
		bool sphere = true;
		bool box = true;
		bool ray = true;
		bool segment = true;
		bool plane = true;
		bool board = true;
		bool capsule = true;
	};

	//2Dの形状の列挙
	enum class ShapeType2D
	{
		RECT,
		CIRCLE,
		SEGMENT,
	};

	//3Dの形状の列挙
	enum class ShapeType3D
	{
		SPHERE,
		BOX,
		PLANE,
		BOARD,
		SEGMENT,
		RAY,
		CAPSULE,
	};


	enum BoxHitDirection
	{
		BOX_HIT_DIRECTION_NO_HIT,
		BOX_HIT_DIRECTION_UP,
		BOX_HIT_DIRECTION_DOWN,
		BOX_HIT_DIRECTION_LEFT,
		BOX_HIT_DIRECTION_RIGHT,
		BOX_HIT_DIRECTION_FRONT,
		BOX_HIT_DIRECTION_BACK,
	};

	//データとリザルトまとめた構造体用意する?両方リサイズするの面倒だし

#pragma region 2D

#pragma region 四角形

//四角形
	struct RectData
	{
		//四角形の左上
		Vector2 position;
		//辺の長さ
		Vector2 size;
	};
#pragma endregion

#pragma region 円


	//円
	struct CircleData
	{
		Vector2 position;
		float r = 0.0f;

	};

	//円の計算結果
	struct CircleCalcResult
	{
		Vector2 lineSegmentNearPosition;
	};
#pragma endregion

#pragma region 線分2D
	enum SegmentHitPlace
	{
		LS_HIT_POSITION_NOT_HIT,//衝突してない
		LS_HIT_POSITION_LE_START_END,//線の端(始点終点)
		LS_HIT_POSITION_LE_LINE,//線の端以外
	};


	struct Segment2DData
	{
		//座標(始点終点)
		Vector2 position[2];


	};

	struct Segment2DCalcResult
	{
		//当たってる座標
		Vector2 hitPos;
		//当たった場所
		SegmentHitPlace hitPlace;

		//最近点
		Vector2 nearPos;
	};


#pragma endregion


#pragma endregion

#pragma region 3D

#pragma region 球


	//球
	struct SphereData
	{
		Vector3 position;

		//半径
		float r = 0.0f;

	};

	struct SphereCalcResult
	{
		//箱のどの部分にぶつかったかという情報
		BoxHitDirection boxHitDistance;
	};

#pragma endregion

#pragma region 箱


	//箱
	struct BoxData
	{
		Vector3 size;
		Vector3 position;

	};

	struct BoxCalcResult
	{

		//相手がどこにぶつかったか
		BoxHitDirection boxHitDistance;
	};

#pragma endregion

#pragma region 平面

	//平面
	struct PlaneData
	{
		Vector3 normal;
		float distance = 0.0f;
	};
#pragma endregion

#pragma region 板


	//板
	struct BoardData
	{
		Vector3 normal;
		Vector3 position;
		Vector3 leftDownPos;
		Vector3 leftUpPos;
		Vector3 rightUpPos;
		Vector3 rightDownPos;

	};

	struct BoardCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

#pragma endregion


#pragma region 線分3D

	//線分
	struct Segment3DData
	{
		Vector3 position[2];
		Vector3 angle = 0;

		

		//これ計算毎フレーム1回にしてCalcResultに入れるようにする?
		//セットしてすぐ取得したい場合あるかもしれないからこのままにする?
		/// <summary>
		/// 線分の中心を基準にangle分回転させた両端の座標を返します。
		/// </summary>
		/// <returns></returns>
		Value2<Vector3>GetRotatePosition()const;

		/// <summary>
		/// 2点間の中心座標を取得します。
		/// </summary>
		/// <returns></returns>
		Vector3 GetCenterPosition ()const;

		/// <summary>
		/// 2点間の距離を求めます。
		/// </summary>
		/// <returns></returns>
		float GetPositionDistance()const;
	};

	struct Segment3DCalcResult
	{
		Vector3 lineSegment3DHitPos;
		Vector3 boardHitPos;
	};

#pragma endregion

#pragma region 線

	//線
	struct RayData
	{
		Vector3 pos;
		Vector3 direction;
	};
#pragma endregion


#pragma region カプセル


	//カプセル
	struct CapsuleData
	{
		Segment3DData segmentData;
		
		float r = 0.0f;
	};

#pragma endregion


#pragma endregion

}