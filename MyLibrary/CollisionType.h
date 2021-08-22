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


	enum class BoxHitDirection
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


	struct SphereCalcResult
	{
		//箱のどの部分にぶつかったかという情報
		BoxHitDirection boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
	};

	//球
	class SphereData
	{
	private:
		//座標
		Vector3 position;

		//半径
		float r = 0.0f;

		SphereCalcResult result;

	public:
		Vector3 GetPosition()const { return position; }
		float GetRadius()const { return r; }
		SphereCalcResult GetCalcResult() const { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetRadius(const float radius) { r = radius; }
		void SetCalcResult(const SphereCalcResult& result) { this->result = result; }
	};


#pragma endregion

#pragma region AABB
	struct BoxCalcResult
	{

		//相手がどこにぶつかったか
		BoxHitDirection boxHitDistance;
	};

	//箱
	class BoxData
	{
	private:
		Vector3 position;
		Vector3 size;
		BoxCalcResult result;

	public:
		Vector3 GetPosition()const{ return position; }
		Vector3 GetSize()const { return size; }
		BoxCalcResult GetCalcResult() { return result; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetSize(const Vector3& size) { this->size = size; }
		void SetCalcResult(const BoxCalcResult& result) { this->result = result; }
	};



#pragma endregion

#pragma region OBB

#pragma endregion

#pragma region 平面

	//平面
	class PlaneData
	{
	private:
		Vector3 normal;
		float distance = 0.0f;

	public:
		Vector3 GetNormal()const { return normal; }
		float GetDistance()const { return distance; }

		void SetNormal(const Vector3& normal) { this->normal = normal; }
		void SetDistance(const float dis) { distance = dis; }
	};
#pragma endregion

#pragma region 板

	struct BoardCalcResult
	{
		Vector3 lineSegment3DHitPos;
	};

	//板
	//板の判定。未回転時は、0,0,1の方向を向きます。
	class BoardData
	{
	private:
		Vector3 position;
		Vector2 size;
		Vector3 angle;
		BoardCalcResult result;

		Vector3 normal = Vector3(0, 0, -1);
		Vector3 leftDownPos;
		Vector3 leftUpPos;
		Vector3 rightUpPos;
		Vector3 rightDownPos;

	public:
		Vector3 GetPosition()const { return position; }
		Vector2 GetSize()const { return size; }
		Vector3 GetAngle()const { return angle; }
		BoardCalcResult GetCalcResult()const { return result; }

		Vector3 GetNormal()const { return normal; }
		//左下、左上、右下、右上の順で格納されている
		Value4<Vector3>GetVertexPosition()const { return Value4<Vector3>(leftDownPos, leftUpPos, rightDownPos, rightUpPos); }


		void SetPosition(const Vector3& pos);
		void SetSize(const Vector2& size);
		void SetAngle(const Vector3& angle);
		void SetCalcResult(const BoardCalcResult& result) { this->result = result; }
	};


#pragma endregion


#pragma region 線分3D
	struct Segment3DCalcResult
	{
		Vector3 lineSegment3DHitPos;
		Vector3 boardHitPos;
	};

	//線分
	class Segment3DData
	{
	private:
		Value2<Vector3> position;
		Vector3 angle = 0;
		//回転基準を決める変数(0.0fでv1基準、0.5fで中心、1.0fでv2基準)
		float rotatePoint = 0.5f;

		//angleを適応させた座標
		Value2<Vector3> rotatePosition;

		Segment3DCalcResult result;

	private:
		//angle適応させた角度をrotatePositionに代入する関数
		void CalcRotatePosition();

		
	public:
		Value2<Vector3> GetPosition()const { return position; }
		Value2<Vector3> GetRotatePosition()const { return rotatePosition; }
		Vector3 GetAngle()const { return angle; }
		float GetRotatePoint()const { return rotatePoint; }
		Segment3DCalcResult GetCalcResult()const { return result; }


		void SetPosition(const Value2<Vector3>& pos);
		void SetAngle(const Vector3& angle);
		/// <summary>
		/// //回転基準を決める数値(0.0fでv1基準、0.5fで中心、1.0fでv2基準)をセットします。
		/// </summary>
		/// <param name="num">回転基準を決める数値(0.0fでv1基準、0.5fで中心、1.0fでv2基準)</param>
		void SetRotatePoint(const float num);
		void SetCalcResult(const Segment3DCalcResult& result) { this->result = result; }
	
	};

	

#pragma endregion

#pragma region レイ

	//線
	class RayData
	{
	private:
		Vector3 position;
		Vector3 direction;

	public:
		Vector3 GetPosition()const { return position; }
		Vector3 GetDirection()const { return direction; }

		void SetPosition(const Vector3& pos) { position = pos; }
		void SetDirection(const Vector3& dir) { direction = dir; }
	};
#pragma endregion


#pragma region カプセル


	//カプセル
	class CapsuleData
	{
	private:
		Segment3DData segmentData;
		float r = 0.0f;

	public:
		Segment3DData GetSegment3DData()const { return segmentData; }
		float GetRadius()const { return r; }

		Segment3DData& GetRefSegment3DData() { return segmentData; }

		void SetSegment3DData(const Segment3DData& data) { segmentData = data; }
		void SetRadius(const float radius) { r = radius; }

	};

#pragma endregion


#pragma endregion

}