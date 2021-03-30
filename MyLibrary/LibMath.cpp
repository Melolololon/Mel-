#include"LibMath.h"
#include"DirectInput.h"


float LibMath::angleConversion(int paterrn, float angle)
{
	if (paterrn == 0)
	{
		return (float)(angle * (M_PI / 180));
	}

	return (float)(angle * (180 / M_PI));

}

bool LibMath::difference(const float& num1, const float& num2, const float& difference)
{
	float sum = num1 - num2;
	float diff = abs(sum);//差を求める

		//差が指定した値以内だったらtrue
	if (difference >= diff)
		return true;

	return false;

}


float LibMath::getFloatPI()
{
	return static_cast<float>(M_PI);
	//return 0;
}

double LibMath::getDoublePI()
{
	return M_PI;
	//return 0;
}

#pragma region ベクトル


#pragma region vector2
char LibMath::pointLeftRightCheck(const Vector2& vector, const Vector2& point)
{
	float num = vector2Cross(vector, point);

	if (num > 0)return 1;
	if (num < 0)return -1;
	return 0;
}

float LibMath::twoVector2Angle(const Vector2& v1, const Vector2& v2)
{

	float f = vector2Dot(v1, { v2.x,v2.y });
	f = std::acos(f);
	f = angleConversion(1, f);

	Vector3 v = vector3Cross({ v1.x,v1.y,0 }, { v2.x,v2.y,0 });
	if (v.z < 0)f = 360 - f;

	return f;
}

float LibMath::vecto2ToAngle(const Vector2& v, const bool& v3)
{
	float f = 0.0f;
	if (v3)
		f = twoVector2Angle({ 1,0 }, v);
	else
		f = twoVector2Angle({ 1,0 }, { v.x,-v.y });
	return f;
}


Vector2 LibMath::angleToVector2(const float& angle, const bool& v3)
{
	Quaternion q = getRotateQuaternion({ 1,0,0 }, { 0,0,1 }, angle);

	if (v3)
		return { q.x,q.y };
	return { q.x,-q.y };
}


Vector2 LibMath::rotateVector2(const Vector2& v, const float& angle)
{
	Quaternion q = getRotateQuaternion({ v.x,v.y,0 }, { 0,0,1 }, angle);
	return { q.x,q.y };
}
#pragma endregion

#pragma region Vector3

float LibMath::calcDistance3D(Vector3 pos1, Vector3 pos2)
{
	return sqrt
	(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z)
	);
}

Vector3 LibMath::otherVector(const Vector3& vec1, const Vector3& vec2)
{
	Vector3 vec;
	vec = vec2 - vec1;
	return vector3Normalize(vec);
}

Vector3 LibMath::rotateVector3(const Vector3& rotateV, const Vector3& vec, const float& angle)
{
	Quaternion q = getRotateQuaternion(rotateV, vec, angle);
	return { q.x, q.y, q.z };
}
#pragma endregion


#pragma endregion


#pragma region 当たり判定
#pragma region 2D

bool LibMath::rectCollision
(
	const Vector2& pos1,
	const Vector2& size1,
	const Vector2& pos2,
	const Vector2& size2
)//四角判定
{
	if (size2.x + pos2.x > pos1.x &&
		pos2.x < size1.x + pos1.x &&
		pos2.y < size1.y + pos1.y &&
		size2.y + pos2.y > pos1.y)
		return true;
	return false;
}

bool LibMath::circleCollision(Vector2 centerPosition1, float r1, Vector2 centerPosition2, float r2)//円判定
{
	if ((centerPosition1.x - centerPosition2.x) *
		(centerPosition1.x - centerPosition2.x) +
		(centerPosition1.y - centerPosition2.y) *
		(centerPosition1.y - centerPosition2.y) <=
		(r1 + r2) * (r1 + r2))
	{
		return true;
	}
	return false;
}

#pragma endregion

#pragma region 3D

bool LibMath::sphereCollision(Vector3 centerPosition1, float r1, Vector3 centerPosition2, float r2)
{
	if ((centerPosition2.x - centerPosition1.x) *
		(centerPosition2.x - centerPosition1.x) +
		(centerPosition2.y - centerPosition1.y) *
		(centerPosition2.y - centerPosition1.y) +
		(centerPosition2.z - centerPosition1.z) *
		(centerPosition2.z - centerPosition1.z) <=
		(r1 + r2) * (r1 + r2))
	{
		return true;
	}
	return false;
}

bool LibMath::sphereAndPlaneCollision(Vector3 spherePos, float r, Vector3 normal, float planeDist, Vector3* hitPos)
{


	//原点から球の距離
	float sphereDist = vector3Dot(spherePos, normal);

	float dict = sphereDist - planeDist;
	if (fabsf(dict) > r)return false;

	if (hitPos) *hitPos = normal * -dict + spherePos;
	return true;
}

bool LibMath::sphereAndTryangleCollision
(
	Vector3 spherePos,
	float r,
	Vector3 triPos1,
	Vector3 triPos2,
	Vector3 triPos3,
	Vector3 normal,
	Vector3* hitPos
)
{
	//最近接点
	Vector3 closest = { 0,0,0 };
	//内積
	float dot1, dot2;
	//所属エリア
	int area = 0;

#pragma region 領域チェック1

	if (area == 0)
	{
		//tryPos1、tryPos2のベクトルと、tryPos1、spherePosのベクトルの内積を求める
		dot1 = vector3Dot(triPos2 - triPos1, spherePos - triPos1);

		//tryPos1、tryPos3のベクトルと、tryPos1、spherePosのベクトルの内積を求める
		dot2 = vector3Dot(triPos3 - triPos1, spherePos - triPos1);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 1;
			closest = triPos1;
		}
	}
#pragma endregion

#pragma region 領域チェック2

	if (area == 0)
	{
		dot1 = vector3Dot(triPos1 - triPos2, spherePos - triPos2);

		dot2 = vector3Dot(triPos3 - triPos2, spherePos - triPos2);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 2;
			closest = triPos2;
		}
	}
#pragma endregion

#pragma region 領域チェック4

	if (area == 0)
	{
		dot1 = vector3Dot(triPos2 - triPos3, spherePos - triPos3);

		dot2 = vector3Dot(triPos1 - triPos3, spherePos - triPos3);

		//dot1とdot2の両方がマイナスなら、領域1にある
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 4;
			closest = triPos3;
		}
	}
#pragma endregion


#pragma region 領域チェック3

	if (area == 0)
	{
		dot1 = vector3Dot(triPos2 - triPos1, spherePos - triPos1);
		dot2 = vector3Dot(triPos2 - triPos1, spherePos - triPos2);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (vector3Dot(triPos2 - triPos1, spherePos - triPos1) * vector3Dot(triPos3 - triPos1, spherePos - triPos2) -
				vector3Dot(triPos2 - triPos1, spherePos - triPos2) * vector3Dot(triPos3 - triPos1, spherePos - triPos1) <= 0)
			{
				area = 3;
				float num =
					vector3Dot(triPos2 - triPos1, spherePos - triPos1) /
					(vector3Dot(triPos2 - triPos1, spherePos - triPos1) - vector3Dot(triPos2 - triPos1, spherePos - triPos2));
				closest.x = triPos1.x + num * (triPos2.x - triPos1.x);
				closest.y = triPos1.y + num * (triPos2.y - triPos1.y);
				closest.z = triPos1.z + num * (triPos2.z - triPos1.z);
			}


		}
	}
#pragma endregion

#pragma region 領域チェック5
	if (area == 0)
	{
		dot1 = vector3Dot(triPos3 - triPos2, spherePos - triPos2);
		dot2 = vector3Dot(triPos3 - triPos2, spherePos - triPos3);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (vector3Dot(triPos3 - triPos2, spherePos - triPos2) * vector3Dot(triPos1 - triPos2, spherePos - triPos3) -
				vector3Dot(triPos3 - triPos2, spherePos - triPos3) * vector3Dot(triPos1 - triPos2, spherePos - triPos2) <= 0)
			{

				area = 5;

				float num =
					vector3Dot(triPos3 - triPos2, spherePos - triPos2) /
					(vector3Dot(triPos3 - triPos2, spherePos - triPos2) - vector3Dot(triPos3 - triPos2, spherePos - triPos3));
				closest.x = triPos2.x + num * (triPos3.x - triPos2.x);
				closest.y = triPos2.y + num * (triPos3.y - triPos2.y);
				closest.z = triPos2.z + num * (triPos3.z - triPos2.z);

			}
		}
	}
#pragma endregion

#pragma region 領域チェック6
	if (area == 0)
	{
		dot1 = vector3Dot(triPos1 - triPos3, spherePos - triPos3);
		dot2 = vector3Dot(triPos1 - triPos3, spherePos - triPos1);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (vector3Dot(triPos1 - triPos3, spherePos - triPos3) * vector3Dot(triPos2 - triPos3, spherePos - triPos1) -
				vector3Dot(triPos1 - triPos3, spherePos - triPos1) * vector3Dot(triPos2 - triPos3, spherePos - triPos3) <= 0)
			{
				area = 6;

				float num =
					vector3Dot(triPos1 - triPos3, spherePos - triPos3) /
					(vector3Dot(triPos1 - triPos3, spherePos - triPos3) - vector3Dot(triPos1 - triPos3, spherePos - triPos1));
				closest.x = triPos3.x + num * (triPos1.x - triPos3.x);
				closest.y = triPos3.y + num * (triPos1.y - triPos3.y);
				closest.z = triPos3.z + num * (triPos1.z - triPos3.z);

			}


		}
	}
#pragma endregion


#pragma region 領域チェック7
	if (area == 0)
	{
		float va =
			vector3Dot(triPos2 - triPos1, spherePos - triPos2) * vector3Dot(triPos3 - triPos1, spherePos - triPos3) -
			vector3Dot(triPos2 - triPos1, spherePos - triPos3) * vector3Dot(triPos3 - triPos1, spherePos - triPos2);

		float vb =
			vector3Dot(triPos2 - triPos1, spherePos - triPos3) * vector3Dot(triPos3 - triPos1, spherePos - triPos1) -
			vector3Dot(triPos2 - triPos1, spherePos - triPos1) * vector3Dot(triPos3 - triPos1, spherePos - triPos3);

		float vc =
			vector3Dot(triPos2 - triPos1, spherePos - triPos1) * vector3Dot(triPos3 - triPos1, spherePos - triPos2) -
			vector3Dot(triPos2 - triPos1, spherePos - triPos2) * vector3Dot(triPos3 - triPos1, spherePos - triPos1);

		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		closest.x = triPos1.x + (triPos2.x - triPos1.x) * v + (triPos3.x - triPos1.x) * w;
		closest.y = triPos1.y + (triPos2.y - triPos1.y) * v + (triPos3.y - triPos1.y) * w;
		closest.z = triPos1.z + (triPos2.z - triPos1.z) * v + (triPos3.z - triPos1.z) * w;

		area = 7;
	}
#pragma endregion


	//最接近点 - 球の中心
	Vector3 v = closest - spherePos;
	float f = vector3Dot(v, v);
	if (f > r * r) return false;

	if (hitPos)*hitPos = closest;
	return true;

}

bool LibMath::sphereAndBoxCollision
(
	const Vector3& spherePos,
	const float& r,
	const Vector3& centerPos,
	const Vector3& size,
	BoxHitDirection* direction
)
{
	float dir2 = 0.0f;

	Vector3 minPos = centerPos - size / 2;
	Vector3 maxPos = centerPos + size / 2;

	//x
	if (spherePos.x < minPos.x)
		dir2 += (minPos.x - spherePos.x) * (minPos.x - spherePos.x);
	if (spherePos.x > maxPos.x)
		dir2 += (spherePos.x - maxPos.x) * (spherePos.x - maxPos.x);

	//y
	if (spherePos.y < minPos.y)
		dir2 += (minPos.y - spherePos.y) * (minPos.y - spherePos.y);
	if (spherePos.y > maxPos.y)
		dir2 += (spherePos.y - maxPos.y) * (spherePos.y - maxPos.y);

	//z
	if (spherePos.z < minPos.z)
		dir2 += (minPos.z - spherePos.z) * (minPos.z - spherePos.z);
	if (spherePos.z > maxPos.z)
		dir2 += (spherePos.z - maxPos.z) * (spherePos.z - maxPos.z);

	bool flag = dir2 < r * r;

	//ボックスのどこに当たったかを返す
	if (direction)
	{
		if (!flag)
		{
			*direction = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
			return flag;
		}

		//1 Xが多い
		//2 Yが多い
		//3 Zが多い
		char top = 0;
		//球へのベクトル
		Vector3 sphereToVector = centerPos - spherePos;

		if (abs(sphereToVector.x) >= abs(sphereToVector.y)) 
		{
			top = 1;
			if (abs(sphereToVector.z) >= abs(sphereToVector.x))
				top = 3;
		}
		else
		{
			top = 2;
			if (abs(sphereToVector.z) >= abs(sphereToVector.y))
				top = 3;
		}

		if (top == 1)
		{
			if (sphereToVector.x >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_LEFT;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_RIGHT;
		}
		if (top == 2)
		{
			if (sphereToVector.y >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_DOWN;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_UP;
		}
		if (top == 3)
		{
			if (sphereToVector.z >= 0)
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_FRONT;
			else
				*direction = BoxHitDirection::BOX_HIT_DIRECTION_BACK;
		}
	}

	return flag;
}

bool LibMath::lineSegmentAndBoardCollision
(
	Vector3 linePos1,
	Vector3 linePos2,
	Vector3 normal,
	Vector3 pointPos,
	std::vector<Vector3>vertexPoint,
	Vector3* crossPosition
)
{

	Vector3 v1;
	Vector3 v2;

	v1 = linePos1 - pointPos;
	v2 = linePos2 - pointPos;

	//線が板ポリと並行ではないかを調べる(平行だったらreturn)
	if (vector3Dot(v1, normal) * vector3Dot(v2, normal) > 0)
		return false;


	//ここにポリゴンの範囲内かどうかの処理を書く

	//線の端 - ポリゴンの角
	v1 = linePos1 - vertexPoint[0];
	v2 = linePos2 - vertexPoint[0];
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

	Vector3 vec1 = linePos1 - pointPos;
	Vector3 vec2 = linePos2 - pointPos;

	float kyori1 = 0;
	if (normal.x != 0)kyori1 += abs(vector3Dot(normal, vec1)) / abs(normal.x);
	if (normal.y != 0)kyori1 += abs(vector3Dot(normal, vec1)) / abs(normal.y);
	if (normal.z != 0)kyori1 += abs(vector3Dot(normal, vec1)) / abs(normal.z);
	float kyori2 = 0;
	if (normal.x != 0)kyori2 += abs(vector3Dot(normal, vec2)) / abs(normal.x);
	if (normal.y != 0)	kyori2 += abs(vector3Dot(normal, vec2)) / abs(normal.y);
	if (normal.z != 0)	kyori2 += abs(vector3Dot(normal, vec2)) / abs(normal.z);

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
	Vector3 crossPos = vertexPoint[0] + crossVector;
	//crossPos.y *= - 1;



	//三角形1個目の判定
	Vector3 normal1;
	Library::calculationNormal(vertexPoint[0], vertexPoint[1], crossPos, normal1, normal1, normal1);
	Vector3 normal2;
	Library::calculationNormal(vertexPoint[1], vertexPoint[2], crossPos, normal2, normal2, normal2);
	Vector3 normal3;
	Library::calculationNormal(vertexPoint[2], vertexPoint[0], crossPos, normal3, normal3, normal3);

	//板ポリと法線が同じか調べる
	bool equal1 = false;//板ポリと法線が同じかどうか

	//同じだったらtrue
	if (difference(normal.x, normal1.x, 0.0001f) &&
		difference(normal.y, normal1.y, 0.0001f) &&
		difference(normal.z, normal1.z, 0.0001f) &&
		difference(normal.x, normal2.x, 0.0001f) &&
		difference(normal.y, normal2.y, 0.0001f) &&
		difference(normal.z, normal2.z, 0.0001f) &&
		difference(normal.x, normal3.x, 0.0001f) &&
		difference(normal.y, normal3.y, 0.0001f) &&
		difference(normal.z, normal3.z, 0.0001f))
	{
		equal1 = true;
	}


	//三角形2個目の判定
	Library::calculationNormal(vertexPoint[2], vertexPoint[1], crossPos, normal1, normal1, normal1);
	Library::calculationNormal(vertexPoint[1], vertexPoint[3], crossPos, normal2, normal2, normal2);
	Library::calculationNormal(vertexPoint[3], vertexPoint[2], crossPos, normal3, normal3, normal3);

	//板ポリと法線が同じか調べる
	bool equal2 = false;//板ポリと法線が同じかどうか

	//同じ(誤差0.0001以内)だったらtrue
	if (difference(normal.x, normal1.x, 0.0001f) &&
		difference(normal.y, normal1.y, 0.0001f) &&
		difference(normal.z, normal1.z, 0.0001f) &&
		difference(normal.x, normal2.x, 0.0001f) &&
		difference(normal.y, normal2.y, 0.0001f) &&
		difference(normal.z, normal2.z, 0.0001f) &&
		difference(normal.x, normal3.x, 0.0001f) &&
		difference(normal.y, normal3.y, 0.0001f) &&
		difference(normal.z, normal3.z, 0.0001f))
	{
		equal2 = true;
	}

	//どちらかが同じ(板ポリの中)だったらifの中に
	if (equal1 || equal2)
	{


		return true;
	}

	//衝突位置と中心が同じだったらヒット
	if (difference(crossPos.x, pointPos.x, 0.01f) &&
		difference(crossPos.y, pointPos.y, 0.01f) &&
		difference(crossPos.z, pointPos.z, 0.01f))
	{
		return true;
	}

	//衝突位置を返す
	if (crossPosition) *crossPosition = { 0,0,0 };

	return false;

}


bool LibMath::rayAndPlaneCollision
(
	Vector3 layStartPos,
	Vector3 layDirection,
	Vector3 normal,
	float planeDistance,
	float* distance,
	Vector3* crossPos
)
{
	const float epsilon = 1.0e-5f;

	float d1 = vector3Dot(normal, layDirection);

	if (d1 > -epsilon) return false;

	float d2 = vector3Dot(normal, layStartPos);

	float dist = d2 - planeDistance;

	float t = dist / -d1;

	if (t < 0)return false;

	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

bool LibMath::rayAndTryangleCollision
(
	Vector3 layStartPos,
	Vector3 layDirection,
	Vector3 triPos1,
	Vector3 triPos2,
	Vector3 triPos3,
	Vector3 normal,
	float* distance,
	Vector3* crossPos
)
{
	Vector3 planeNormal;
	float planeDistance;
	Vector3 interPlane;

	planeNormal = normal;
	planeDistance = vector3Dot(normal, triPos1);

	if (!rayAndPlaneCollision(layStartPos, layDirection, planeNormal, planeDistance, distance, &interPlane))return false;



	//交差判定
	const float epsilon = 1.0e-5f;
	Vector3 m;

	Vector3 pt_p0 = triPos1 - interPlane;
	Vector3 p0_p1 = triPos2 - triPos1;
	m = vector3Cross(pt_p0, p0_p1);
	if (vector3Dot(m, normal) < -epsilon)return false;


	Vector3 pt_p1 = triPos2 - interPlane;
	Vector3 p1_p2 = triPos3 - triPos2;
	m = vector3Cross(pt_p1, p1_p2);
	if (vector3Dot(m, normal) < -epsilon)return false;


	Vector3 pt_p2 = triPos3 - interPlane;
	Vector3 p2_p0 = triPos1 - triPos3;
	m = vector3Cross(pt_p2, p2_p0);
	if (vector3Dot(m, normal) < -epsilon)return false;

	if (crossPos)*crossPos = interPlane;
	return true;

}

bool LibMath::rayAndSphereCollision
(
	Vector3 layStartPos,
	Vector3 layDirection,
	Vector3 spherePos,
	float r,
	float* distance,
	Vector3* crossPos
)
{
	Vector3 m = layStartPos - spherePos;
	float b = vector3Dot(m, layDirection);
	float c = vector3Dot(m, m) - r * r;
	if (c > 0.0f && b > 0.0f)return false;

	float discr = b * b - c;
	if (discr < 0.0f)return false;


	float t = -b - sqrt(discr);
	if (t < 0)t = 0.0f;
	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

#pragma endregion

#pragma endregion

