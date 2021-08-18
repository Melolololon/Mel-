#include "Collision.h"
#include"LibMath.h"
#include"Values.h"

using namespace MelLib;



#pragma region 2D



bool Collision::RectAndRect(const RectData& rect1, const RectData& rect2)
{
	return rect2.size.x + rect2.position.x > rect1.position.x &&
		rect2.position.x < rect1.size.x + rect2.position.x &&
		rect2.position.y < rect1.size.y + rect1.position.y &&
		rect2.size.y + rect2.position.y > rect1.position.y;
}

bool Collision::CircleAndCircle(const CircleData& circle1, const CircleData& circle2)
{
	return (circle1.position.x - circle2.position.x) *
		(circle1.position.x - circle2.position.x) +
		(circle1.position.y - circle2.position.y) *
		(circle1.position.y - circle2.position.y) <=
		(circle1.r + circle2.r) * (circle1.r + circle2.r);
}

bool Collision::RectAndCircle(const RectData& rect, const CircleData& circle)
{
	return false;
}

bool Collision::CircleAndSegment2D
(
	const CircleData& circle,
	CircleCalcResult* circleCalcResult,
	const Segment2DData& lineSegment,
	Segment2DCalcResult* lineSegmentCalcResult
)
{
	SegmentHitPlace hitPlace = SegmentHitPlace::LS_HIT_POSITION_NOT_HIT;
	if (lineSegmentCalcResult) 
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	//線の端から端へのベクトル
	Vector2 lineVector = lineSegment.position[1] - lineSegment.position[0];
	//線の座標1から円の中心
	Vector2 linePos0ToSphere = circle.position - lineSegment.position[0];
	//円と線の最短距離を求める
	float distance = abs(Vector2::Cross(lineVector, linePos0ToSphere) / Vector2::Length(lineVector));


	//円と線分の最近点を求める
	//これ関数化する

	//linePos1から最近点の距離を求める
	float linePos0ToNearPosDis = Vector2::Dot(Vector2::Normalize(lineVector), linePos0ToSphere);
	if (lineSegmentCalcResult) 
	{
		lineSegmentCalcResult->nearPos = lineSegment.position[0] + (Vector2::Normalize(lineVector) * linePos0ToNearPosDis);
	}

	//距離のほうが大きかったら計算終了
	if (distance > circle.r)return false;

	//線の座標2から円の中心
	Vector2 linePos2ToSphere = circle.position - lineSegment.position[1];
	//正の数かどうかを判別するフラグ
	bool linePos1ToSpherePositive = false;
	bool linePos2ToSpherePositive = false;

	//内積を求める
	float linePos1ToSphereDot = Vector2::Dot(lineVector, linePos0ToSphere);
	float linePos2ToSphereDot = Vector2::Dot(lineVector, linePos2ToSphere);

	//正か負か確認
	if (linePos1ToSphereDot >= 0)
		linePos1ToSpherePositive = true;
	if (linePos2ToSphereDot >= 0)
		linePos2ToSpherePositive = true;


	//どちらも正または負だったら入る
	if (linePos1ToSpherePositive
		&& linePos2ToSpherePositive
		|| !linePos1ToSpherePositive
		&& !linePos2ToSpherePositive)
	{
		//大きさを求める
		float linePos1ToSphereLength = Vector2::Length(linePos0ToSphere);
		float linePos2ToSphereLength = Vector2::Length(linePos2ToSphere);

		//半径より大きかったら当たってない
		if (linePos1ToSphereLength > circle.r
			&& linePos2ToSphereLength > circle.r)
		{
			return false;
		}

		hitPlace = SegmentHitPlace::LS_HIT_POSITION_LE_START_END;
	}
	else
	{
		hitPlace = SegmentHitPlace::LS_HIT_POSITION_LE_LINE;
	}

	if(lineSegmentCalcResult)
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	return true;
}


#pragma endregion

#pragma region 3D

bool Collision::SphereAndSphere(const SphereData& sphere1, const SphereData& sphere2)
{

	return (sphere2.position.x - sphere1.position.x) *
		(sphere2.position.x - sphere1.position.x) +
		(sphere2.position.y - sphere1.position.y) *
		(sphere2.position.y - sphere1.position.y) +
		(sphere2.position.z - sphere1.position.z) *
		(sphere2.position.z - sphere1.position.z) <=
		(sphere1.r + sphere2.r) * (sphere1.r + sphere2.r);
}

bool Collision::BoxAndBox
(
	const BoxData& box1, 
	BoxCalcResult* boxCalcResult1,
	const BoxData& box2, 
	BoxCalcResult* boxCalcResult2
)
{
	Vector3 minPos1 = box1.position - box1.size / 2;
	Vector3 maxPos1 = box1.position + box1.size / 2;
	Vector3 minPos2 = box2.position - box2.size / 2;
	Vector3 maxPos2 = box2.position + box2.size / 2;
	bool isHit = false;

	//1か2のminかmaxがXYZ全部相手の範囲内だったら当たってる
	if (minPos1.x >= minPos2.x &&
		minPos1.x < maxPos2.x ||
		maxPos1.x >= minPos2.x &&
		maxPos1.x < maxPos2.x ||
		minPos2.x >= minPos1.x &&
		minPos2.x < maxPos1.x ||
		maxPos2.x >= minPos1.x &&
		maxPos2.x < maxPos1.x)
	{

		if (minPos1.y >= minPos2.y &&
			minPos1.y < maxPos2.y ||
			maxPos1.y >= minPos2.y &&
			maxPos1.y < maxPos2.y ||
			minPos2.y >= minPos1.y &&
			minPos2.y < maxPos1.y ||
			maxPos2.y >= minPos1.y &&
			maxPos2.y < maxPos1.y)
		{
			if (minPos1.z >= minPos2.z &&
				minPos1.z < maxPos2.z ||
				maxPos1.z >= minPos2.z &&
				maxPos1.z < maxPos2.z ||
				minPos2.z >= minPos1.z &&
				minPos2.z < maxPos1.z ||
				maxPos2.z >= minPos1.z &&
				maxPos2.z < maxPos1.z) 
			{
				isHit = true;
			}

		}
	}

	if (boxCalcResult1 || boxCalcResult2)
	{
		BoxHitDirection direction1 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		BoxHitDirection direction2 = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;

		if (!isHit)
		{
			if (boxCalcResult1) boxCalcResult1->boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			if (boxCalcResult2) boxCalcResult2->boxHitDistance = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return false;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//対象へのベクトル
		Vector3 targetToVector = box2.position - box1.position;

		if (abs(targetToVector.x) > abs(targetToVector.y) &&
			abs(targetToVector.x) > box2.size.x / 2)
		{
			top = 1;
			if (abs(targetToVector.z) > abs(targetToVector.x) &&
				abs(targetToVector.z) > box2.size.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(targetToVector.z) > abs(targetToVector.y) &&
				abs(targetToVector.z) > box2.size.z / 2)
				top = 3;
		}


		if (top == 1)
		{
			if (targetToVector.x >= 0)
			{
				
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		else if (top == 2)
		{
			if (targetToVector.y >= 0)
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		else if (top == 3)
		{
			if (targetToVector.z >= 0)
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				direction1 = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
				direction2 = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}

		if (boxCalcResult1) boxCalcResult1->boxHitDistance = direction1;
		if (boxCalcResult2) boxCalcResult2->boxHitDistance = direction2;
	}

	return isHit;
}

bool Collision::Segment3DAndSegment3D
(
	const Segment3DData& lineSegment1,
	Segment3DCalcResult* lineSegment1CalcResult,
	const Segment3DData& lineSegment2,
	Segment3DCalcResult* lineSegment2CalcResult
)
{
	Value2<Vector3>segmentPos1 = lineSegment1.GetRotatePosition();

	Value2<Vector3>segmentPos2 = lineSegment2.GetRotatePosition();


	Vector3 d1 = segmentPos1.v2 - segmentPos1.v1;
	Vector3 d2 = segmentPos2.v2 - segmentPos2.v1;
	//カプセルの処理の一部を使えば求められる?
	Vector3 r = segmentPos1.v1 - segmentPos2.v1;
	float a = Vector3::Dot(d1, d1);
	float b = Vector3::Dot(d1, d2);
	float c = Vector3::Dot(d1, r);
	float e = Vector3::Dot(d2, d2);
	float f = Vector3::Dot(d2, r);
	float d = a * e - b * b;

	float s = 0.0f;
	if (d != 0.0f)
	{
		s = (b * f - c * e) / d;

		if (s < 0.0f)s = 0.0f;
		if (s > 1.0f)s = 1.0f;
	}

	Vector3 p1 = segmentPos1.v1;
	Vector3 p2 = segmentPos2.v1;
	float t = Vector3::Dot(((p1 + d1 * s) - p2), d2) / Vector3::Dot(d2, d2);

	s = (t * b - c) / a;
	if (t < 0.0f)
	{
		t = 0.0f;
		s = -c / a;
	}
	else if (t > 1.0f)
	{
		t = 1.0f;
		s = (b - c) / a;
	}
	if (s < 0.0f)s = 0.0f;
	if (s > 1.0f)s = 1.0f;

	//相手に一番近い場所
	Vector3 c1 = segmentPos1.v1 + s * d1;
	Vector3 c2 = segmentPos2.v1 + t * d2;

	//近い場所の距離が0だったら当たっている(重なっている、交差している)
	bool isHit = LibMath::CalcDistance3D(c1, c2) == 0.0f;
	
	if (lineSegment1CalcResult)lineSegment1CalcResult->lineSegment3DHitPos = c1;
	if (lineSegment2CalcResult)lineSegment2CalcResult->lineSegment3DHitPos = c2;
	return isHit;

}

bool Collision::CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2)
{
	//回転適応.
	Value2<Vector3>capsule1LineSegmentPos = capsule1.segmentData.GetRotatePosition();

	Value2<Vector3>capsule2LineSegmentPos = capsule2.segmentData.GetRotatePosition();


	Vector3 d1 = capsule1LineSegmentPos.v2 - capsule1LineSegmentPos.v1;
	Vector3 d2 = capsule2LineSegmentPos.v2 - capsule2LineSegmentPos.v1;

	Vector3 r = capsule1LineSegmentPos.v1 - capsule2LineSegmentPos.v1;
	float a = Vector3::Dot(d1, d1);
	float b = Vector3::Dot(d1, d2);
	float c = Vector3::Dot(d1, r);
	float e = Vector3::Dot(d2, d2);
	float f = Vector3::Dot(d2, r);
	float d = a * e - b * b;

	float s = 0.0f;
	if (d != 0.0f)
	{
		s = (b * f - c * e) / d;

		if (s < 0.0f)s = 0.0f;
		if (s > 1.0f)s = 1.0f;
	}

	Vector3 p1 = capsule1LineSegmentPos.v1;
	Vector3 p2 = capsule2LineSegmentPos.v1;
	float t = Vector3::Dot(((p1 + d1 * s) - p2), d2) / Vector3::Dot(d2, d2);

	s = (t * b - c) / a;
	if(t < 0.0f)
	{
		t = 0.0f;
		s = -c / a;
	}
	else if(t > 1.0f)
	{
		t = 1.0f;
		s = (b - c) / a;
	}
	if (s < 0.0f)s = 0.0f;
	if (s > 1.0f)s = 1.0f;

	//相手カプセルに一番近い場所
	Vector3 c1 = p1 + s * d1;
	Vector3 c2 = p2 + t * d2;

	return LibMath::CalcDistance3D(c1,c2) < capsule1.r + capsule2.r;
}

bool Collision::SphereAndBox
(
	const SphereData& sphere, 
	SphereCalcResult* sphereCalcResult, 
	const BoxData& box, 
	BoxCalcResult* boxCalcResult
)
{
	float dir2 = 0.0f;

	Vector3 minPos = box.position - box.size / 2;
	Vector3 maxPos = box.position + box.size / 2;

	//x
	if (sphere.position.x < minPos.x)
		dir2 += (minPos.x - sphere.position.x) * (minPos.x - sphere.position.x);
	if (sphere.position.x > maxPos.x)
		dir2 += (sphere.position.x - maxPos.x) * (sphere.position.x - maxPos.x);

	//y
	if (sphere.position.y < minPos.y)
		dir2 += (minPos.y - sphere.position.y) * (minPos.y - sphere.position.y);
	if (sphere.position.y > maxPos.y)
		dir2 += (sphere.position.y - maxPos.y) * (sphere.position.y - maxPos.y);

	//z
	if (sphere.position.z < minPos.z)
		dir2 += (minPos.z - sphere.position.z) * (minPos.z - sphere.position.z);
	if (sphere.position.z > maxPos.z)
		dir2 += (sphere.position.z - maxPos.z) * (sphere.position.z - maxPos.z);

	bool isHit = dir2 < sphere.r * sphere.r;

	//どこに当たったかを返す
	if (sphereCalcResult || boxCalcResult)
	{
		//箱のどの部分にぶつかったかという情報
		BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		if (!isHit)
		{
			sphereCalcResult->boxHitDistance = hitDirection;
			boxCalcResult->boxHitDistance = hitDirection;
			return false;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//ボックスへのベクトル
		Vector3 sphereToVector = box.position - sphere.position;

		if (abs(sphereToVector.x) > abs(sphereToVector.y) &&
			abs(sphereToVector.x) > box.size.x / 2)
		{
			top = 1;
			if (abs(sphereToVector.z) > abs(sphereToVector.x) &&
				abs(sphereToVector.z) > box.size.z / 2)
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) > abs(sphereToVector.y) &&
				abs(sphereToVector.z) > box.size.z / 2)
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
			}
		}
		else if (top == 2)
		{
			if (sphereToVector.y >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_UP;
			}
		}
		else if (top == 3)
		{
			if (sphereToVector.z >= 0)
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			}
			else
			{
				hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
			}
		}

		if(sphereCalcResult)sphereCalcResult->boxHitDistance = hitDirection;
		if(boxCalcResult)boxCalcResult->boxHitDistance = hitDirection;
	}

	return isHit;
}

bool Collision::SphereAndCapsule(const SphereData& sphere, const CapsuleData& capsule)
{
	Value2<Vector3>capsuleLineSegmentPos = capsule.segmentData.GetRotatePosition();


	Vector3 capsulePos0ToSphere = 
		LibMath::OtherVector(capsuleLineSegmentPos.v1, sphere.position);
	
	//資料のn
	Vector3 capsuleLineSegmentVector =
		LibMath::OtherVector(capsuleLineSegmentPos.v1, capsuleLineSegmentPos.v2);

	float t = Vector3::Dot(capsulePos0ToSphere, capsuleLineSegmentVector);

	//球から線分に垂直に下した線上の座標 資料のPs→Pn
	Vector3 onTheLinePos = capsuleLineSegmentVector * t;

	float lenghtRate = t * onTheLinePos.Length();
	float sphereAndCupsuleDis = 0.0f;
	if(lenghtRate < 0.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(sphere.position, capsuleLineSegmentPos.v1);
	}
	else if(lenghtRate > 1.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(sphere.position, capsuleLineSegmentPos.v2);
	}
	else
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(sphere.position, onTheLinePos);
	}

	return sphereAndCupsuleDis < sphere.r;
}

bool Collision::BoardAndLineSegment3D
(
	const BoardData& board,
	BoardCalcResult* boardCalcResult,
	const Segment3DData& lineSegment,
	Segment3DCalcResult* lineSegmentCalcResult
)
{


	Vector3 v1;
	Vector3 v2;

	v1 = lineSegment.position[0] - board.position;
	v2 = lineSegment.position[1] - board.position;

	//線が板ポリと並行ではないかを調べる(平行だったらreturn)
	if (Vector3Dot(v1, board.normal) * Vector3Dot(v2, board.normal) > 0)
		return false;


	//ここにポリゴンの範囲内かどうかの処理を書く

	//線の端 - ポリゴンの角
	v1 = lineSegment.position[0] - board.leftDownPos;
	v2 = lineSegment.position[1] - board.leftDownPos;
	/*v1 = normalize(v1);
	v2 = normalize(v2);*/


	//ここがおかしい?

	////線の端から板ポリまでの距離を求める
	//float d = dot(normal,pointPos);
	//float kyori1;//平面から、linePos1までの距離
	//kyori1 = 
	//	abs(normal.x * linePos1.x + normal.y * linePos1.y +normal.z * linePos1.z + d) / 
	//	sqrt(normal.x* normal.x + normal.y* normal.y + normal.z* normal.z);

	//float kyori2;
	//kyori2 = 
	//	abs(normal.x * linePos2.x + normal.y * linePos2.y + normal.z * linePos2.z + d) /
	//	sqrt(normal.x* normal.x + normal.y* normal.y + normal.z* normal.z);

	Vector3 vec1 = lineSegment.position[0] - board.position;
	Vector3 vec2 = lineSegment.position[1] - board.position;

	float kyori1 = 0;
	if (board.normal.x != 0)kyori1 += abs(Vector3Dot(board.normal, vec1)) / abs(board.normal.x);
	if (board.normal.y != 0)kyori1 += abs(Vector3Dot(board.normal, vec1)) / abs(board.normal.y);
	if (board.normal.z != 0)kyori1 += abs(Vector3Dot(board.normal, vec1)) / abs(board.normal.z);
	float kyori2 = 0;
	if (board.normal.x != 0)kyori2 += abs(Vector3Dot(board.normal, vec2)) / abs(board.normal.x);
	if (board.normal.y != 0)	kyori2 += abs(Vector3Dot(board.normal, vec2)) / abs(board.normal.y);
	if (board.normal.z != 0)	kyori2 += abs(Vector3Dot(board.normal, vec2)) / abs(board.normal.z);

	/*float kyori1 = abs(dot(normal, linePos1)) / abs(normal.x);
	float kyori2 = abs(dot(normal, linePos2)) / abs(normal.x);*/



	//内分比
	float a;
	a = kyori1 / (kyori1 + kyori2);


	//ここおかしい

	//線とポリゴンが当たっている場所を調べる
	Vector3 crossVector;//ポリゴンの角から当たってる場所の座標へのベクトル
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = board.leftDownPos + crossVector;
	//crossPos.y *= - 1;



	//三角形1個目の判定
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(board.leftDownPos, board.leftUpPos, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(board.leftUpPos, board.rightDownPos, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(board.rightDownPos, board.leftDownPos, crossPos);

	//板ポリと法線が同じか調べる
	bool equal1 = false;//板ポリと法線が同じかどうか

	//ほぼ同じだったらtrue
	if (LibMath::Difference(board.normal.x, normal1.x, 0.0001f) &&
		LibMath::Difference(board.normal.y, normal1.y, 0.0001f) &&
		LibMath::Difference(board.normal.z, normal1.z, 0.0001f) &&
		LibMath::Difference(board.normal.x, normal2.x, 0.0001f) &&
		LibMath::Difference(board.normal.y, normal2.y, 0.0001f) &&
		LibMath::Difference(board.normal.z, normal2.z, 0.0001f) &&
		LibMath::Difference(board.normal.x, normal3.x, 0.0001f) &&
		LibMath::Difference(board.normal.y, normal3.y, 0.0001f) &&
		LibMath::Difference(board.normal.z, normal3.z, 0.0001f))
	{
		equal1 = true;
	}

	/*p[0] = c2.leftDownPos;
p[1] = c2.leftUpPos;
p[2] = c2.rightDownPos;
p[3] = c2.rightUpPos;*/

	//三角形2個目の判定
	normal1 = LibMath::CalcNormal(board.rightDownPos, board.leftUpPos, crossPos);
	normal2 = LibMath::CalcNormal(board.leftUpPos, board.rightUpPos, crossPos);
	normal3 = LibMath::CalcNormal(board.rightUpPos, board.rightDownPos, crossPos);

	//板ポリと法線が同じか調べる
	bool equal2 = false;//板ポリと法線が同じかどうか

	//ほぼ同じ(誤差0.0001以内)だったらtrue
	if (LibMath::Difference(board.normal.x, normal1.x, 0.0001f) &&
		LibMath::Difference(board.normal.y, normal1.y, 0.0001f) &&
		LibMath::Difference(board.normal.z, normal1.z, 0.0001f) &&
		LibMath::Difference(board.normal.x, normal2.x, 0.0001f) &&
		LibMath::Difference(board.normal.y, normal2.y, 0.0001f) &&
		LibMath::Difference(board.normal.z, normal2.z, 0.0001f) &&
		LibMath::Difference(board.normal.x, normal3.x, 0.0001f) &&
		LibMath::Difference(board.normal.y, normal3.y, 0.0001f) &&
		LibMath::Difference(board.normal.z, normal3.z, 0.0001f))
	{
		equal2 = true;
	}

	//どちらかが同じ(板ポリの中)だったらifの中に
	if (equal1 || equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->lineSegment3DHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}

	//衝突位置と中心がほぼ同じだったらヒット
	if (LibMath::Difference(crossPos.x, board.position.x, 0.001f)
		&& LibMath::Difference(crossPos.y, board.position.y, 0.001f)
		&& LibMath::Difference(crossPos.z, board.position.z, 0.001f)
		|| equal1
		|| equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->boardHitPos = crossPos;
		if (boardCalcResult)boardCalcResult->lineSegment3DHitPos = crossPos;
		return true;
	}
	return false;

}


#pragma endregion


