#pragma once
#include"Vector.h"
#include"Values.h"
namespace MelLib
{

	//CalcResultをCollisionResultにする?

	//衝突確認フラグ
	struct CollisionDetectionFlag
	{
		bool sphere = false;
		bool box = false;
		bool ray = false;
		bool lineSegment = false;
		bool plane = false;
		bool board = false;
		bool capsule = false;
	};

	//2Dの形状の列挙
	enum class ShapeType2D
	{
		RECT,
		CIRCLE,
		LINE_SEGMENT,
	};

	//3Dの形状の列挙
	enum class ShapeType3D
	{
		SPHERE,
		BOX,
		PLANE,
		BOARD,
		LINE_SEGMENT,
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
	enum LineSegmentHitPlace
	{
		LS_HIT_POSITION_NOT_HIT,//衝突してない
		LS_HIT_POSITION_LE_START_END,//線の端(始点終点)
		LS_HIT_POSITION_LE_LINE,//線の端以外
	};


	struct LineSegment2DData
	{
		//座標(始点終点)
		Vector2 position[2];


	};

	struct LineSegment2DCalcResult
	{
		//当たってる座標
		Vector2 hitPos;
		//当たった場所
		LineSegmentHitPlace hitPlace;

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
	struct LineSegment3DData
	{
		Vector3 position[2];

	};

	struct LineSegment3DCalcResult
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
		/// <summary>
		/// カプセルの線分の両端を求める関数。
		/// </summary>
		/// <param name="capsule"></param>
		/// <returns></returns>
		Value2<Vector3>CalcCapsuleLineSegmentPos()const;


		//上の座標から下の座標の距離
		float length = 0.0f;
		float r = 0.0f;
		Vector3 angle = 0;
		Vector3 position = 0;
	};

#pragma endregion


#pragma endregion

}