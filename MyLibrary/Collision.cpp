#include "Collision.h"
#include"LibMath.h"

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

bool Collision::CircleAndLineSegment2D
(
	const CircleData& circle,
	CircleCalcResult* circleCalcResult,
	const LineSegment2DData& lineSegment,
	LineSegment2DCalcResult* lineSegmentCalcResult
)
{
	LineSegmentHitPlace hitPlace = LineSegmentHitPlace::LS_HIT_POSITION_NOT_HIT;
	if (lineSegmentCalcResult) 
	{
		lineSegmentCalcResult->hitPlace = hitPlace;
	}

	//���̒[����[�ւ̃x�N�g��
	Vector2 lineVector = lineSegment.position[1] - lineSegment.position[0];
	//���̍��W1����~�̒��S
	Vector2 linePos0ToSphere = circle.position - lineSegment.position[0];
	//�~�Ɛ��̍ŒZ���������߂�
	float distance = abs(Vector2::Cross(lineVector, linePos0ToSphere) / Vector2::Length(lineVector));


	//�~�Ɛ����̍ŋߓ_�����߂�
	//����֐�������

	//linePos1����ŋߓ_�̋��������߂�
	float linePos0ToNearPosDis = Vector2::Dot(Vector2::Normalize(lineVector), linePos0ToSphere);
	if (lineSegmentCalcResult) 
	{
		lineSegmentCalcResult->nearPos = lineSegment.position[0] + (Vector2::Normalize(lineVector) * linePos0ToNearPosDis);
	}

	//�����̂ق����傫��������v�Z�I��
	if (distance > circle.r)return false;

	//���̍��W2����~�̒��S
	Vector2 linePos2ToSphere = circle.position - lineSegment.position[1];
	//���̐����ǂ����𔻕ʂ���t���O
	bool linePos1ToSpherePositive = false;
	bool linePos2ToSpherePositive = false;

	//���ς����߂�
	float linePos1ToSphereDot = Vector2::Dot(lineVector, linePos0ToSphere);
	float linePos2ToSphereDot = Vector2::Dot(lineVector, linePos2ToSphere);

	//���������m�F
	if (linePos1ToSphereDot >= 0)
		linePos1ToSpherePositive = true;
	if (linePos2ToSphereDot >= 0)
		linePos2ToSpherePositive = true;


	//�ǂ�������܂��͕������������
	if (linePos1ToSpherePositive
		&& linePos2ToSpherePositive
		|| !linePos1ToSpherePositive
		&& !linePos2ToSpherePositive)
	{
		//�傫�������߂�
		float linePos1ToSphereLength = Vector2::Length(linePos0ToSphere);
		float linePos2ToSphereLength = Vector2::Length(linePos2ToSphere);

		//���a���傫�������瓖�����ĂȂ�
		if (linePos1ToSphereLength > circle.r
			&& linePos2ToSphereLength > circle.r)
		{
			return false;
		}

		hitPlace = LineSegmentHitPlace::LS_HIT_POSITION_LE_START_END;
	}
	else
	{
		hitPlace = LineSegmentHitPlace::LS_HIT_POSITION_LE_LINE;
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

	//1��2��min��max��XYZ�S������͈͓̔��������瓖�����Ă�
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

		//1 X������
		//2 Y������
		//3 Z������
		char top = 0;
		//�Ώۂւ̃x�N�g��
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

bool Collision::LineSegment3DAndLineSegment3D
(
	const LineSegment3DData& lineSegment1,
	LineSegment3DCalcResult* lineSegment1CalcResult,
	const LineSegment3DData& lineSegment2,
	LineSegment3DCalcResult* lineSegment2CalcResult
)
{
	Vector3 d1 = lineSegment1.position[1] - lineSegment1.position[0];
	Vector3 d2 = lineSegment2.position[1] - lineSegment2.position[0];
	//�J�v�Z���̏����̈ꕔ���g���΋��߂���?
	Vector3 r = lineSegment1.position[0] - lineSegment2.position[0];
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

	Vector3 p1 = lineSegment1.position[0];
	Vector3 p2 = lineSegment2.position[0];
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

	//����Ɉ�ԋ߂��ꏊ
	Vector3 c1 = lineSegment1.position[0] + s * d1;
	Vector3 c2 = lineSegment2.position[0] + t * d2;

	//�߂��ꏊ�̋�����0�������瓖�����Ă���(�d�Ȃ��Ă���A�������Ă���)
	bool isHit = LibMath::CalcDistance3D(c1, c2) == 0.0f;
	
	if (lineSegment1CalcResult)lineSegment1CalcResult->hitPos = c1;
	if (lineSegment2CalcResult)lineSegment2CalcResult->hitPos = c2;
	return isHit;

}

bool Collision::CapsuleAndCapsule(const CapsuleData& capsule1, const CapsuleData& capsule2)
{

	//����̍��W
	Vector3 capculeLinePos[2] = { 0.0f,0.0f };

	Vector3 d1 = capsule1.lineSegmentData.position[1] - capsule1.lineSegmentData.position[0];
	Vector3 d2 = capsule2.lineSegmentData.position[1] - capsule2.lineSegmentData.position[0];

	Vector3 r = capsule1.lineSegmentData.position[0] - capsule2.lineSegmentData.position[0];
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

	Vector3 p1 = capsule1.lineSegmentData.position[0];
	Vector3 p2 = capsule2.lineSegmentData.position[0];
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

	//����J�v�Z���Ɉ�ԋ߂��ꏊ
	Vector3 c1 = capsule1.lineSegmentData.position[0] + s * d1;
	Vector3 c2 = capsule2.lineSegmentData.position[0] + t * d2;

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

	//�ǂ��ɓ�����������Ԃ�
	if (sphereCalcResult || boxCalcResult)
	{
		//���̂ǂ̕����ɂԂ��������Ƃ������
		BoxHitDirection hitDirection = BoxHitDirection::BOX_HIT_DIRECTION_NO_HIT;
		if (!isHit)
		{
			sphereCalcResult->boxHitDistance = hitDirection;
			boxCalcResult->boxHitDistance = hitDirection;
			return false;
		}

		//1 X������
		//2 Y������
		//3 Z������
		char top = 0;
		//�{�b�N�X�ւ̃x�N�g��
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
	Vector3 capsulePos0ToSphere = 
		LibMath::OtherVector(capsule.lineSegmentData.position[0], sphere.position);
	
	//������n
	Vector3 capsuleLineSegmentVector =
		LibMath::OtherVector(capsule.lineSegmentData.position[0], capsule.lineSegmentData.position[1]);

	float t = Vector3::Dot(capsulePos0ToSphere, capsuleLineSegmentVector);

	//����������ɐ����ɉ���������̍��W ������Ps��Pn
	Vector3 onTheLinePos = capsuleLineSegmentVector * t;

	float lenghtRate = t * onTheLinePos.Length();
	float sphereAndCupsuleDis = 0.0f;
	if(lenghtRate < 0.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(sphere.position, capsule.lineSegmentData.position[0]);
	}
	else if(lenghtRate > 1.0f)
	{
		sphereAndCupsuleDis = LibMath::CalcDistance3D(sphere.position, capsule.lineSegmentData.position[1]);
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
	const LineSegment3DData& lineSegment,
	LineSegment3DCalcResult* lineSegmentCalcResult
)
{


	Vector3 v1;
	Vector3 v2;

	v1 = lineSegment.position[0] - board.position;
	v2 = lineSegment.position[1] - board.position;

	//�����|���ƕ��s�ł͂Ȃ����𒲂ׂ�(���s��������return)
	if (Vector3Dot(v1, board.normal) * Vector3Dot(v2, board.normal) > 0)
		return false;


	//�����Ƀ|���S���͈͓̔����ǂ����̏���������

	//���̒[ - �|���S���̊p
	v1 = lineSegment.position[0] - board.leftDownPos;
	v2 = lineSegment.position[1] - board.leftDownPos;
	/*v1 = normalize(v1);
	v2 = normalize(v2);*/


	//��������������?

	////���̒[����|���܂ł̋��������߂�
	//float d = dot(normal,pointPos);
	//float kyori1;//���ʂ���AlinePos1�܂ł̋���
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



	//������
	float a;
	a = kyori1 / (kyori1 + kyori2);


	//������������

	//���ƃ|���S�����������Ă���ꏊ�𒲂ׂ�
	Vector3 crossVector;//�|���S���̊p���瓖�����Ă�ꏊ�̍��W�ւ̃x�N�g��
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = board.leftDownPos + crossVector;
	//crossPos.y *= - 1;



	//�O�p�`1�ڂ̔���
	Vector3 normal1;
	normal1 = LibMath::CalcNormal(board.leftDownPos, board.leftUpPos, crossPos);
	Vector3 normal2;
	normal2 = LibMath::CalcNormal(board.leftUpPos, board.rightDownPos, crossPos);
	Vector3 normal3;
	normal3 = LibMath::CalcNormal(board.rightDownPos, board.leftDownPos, crossPos);

	//�|���Ɩ@�������������ׂ�
	bool equal1 = false;//�|���Ɩ@�����������ǂ���

	//�قړ�����������true
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

	//�O�p�`2�ڂ̔���
	normal1 = LibMath::CalcNormal(board.rightDownPos, board.leftUpPos, crossPos);
	normal2 = LibMath::CalcNormal(board.leftUpPos, board.rightUpPos, crossPos);
	normal3 = LibMath::CalcNormal(board.rightUpPos, board.rightDownPos, crossPos);

	//�|���Ɩ@�������������ׂ�
	bool equal2 = false;//�|���Ɩ@�����������ǂ���

	//�قړ���(�덷0.0001�ȓ�)��������true
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

	//�ǂ��炩������(�|���̒�)��������if�̒���
	if (equal1 || equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->hitPos = crossPos;
		if (boardCalcResult)boardCalcResult->hitPos = crossPos;
		return true;
	}

	//�Փˈʒu�ƒ��S���قړ�����������q�b�g
	if (LibMath::Difference(crossPos.x, board.position.x, 0.001f)
		&& LibMath::Difference(crossPos.y, board.position.y, 0.001f)
		&& LibMath::Difference(crossPos.z, board.position.z, 0.001f)
		|| equal1
		|| equal2)
	{
		if (lineSegmentCalcResult) lineSegmentCalcResult->hitPos = crossPos;
		if (boardCalcResult)boardCalcResult->hitPos = crossPos;
		return true;
	}
	return false;

}


#pragma endregion


