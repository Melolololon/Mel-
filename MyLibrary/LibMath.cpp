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

bool LibMath::difference(float num1, float num2, float difference, int type)
{
	float sum = num1 - num2;
	float diff = abs(sum);//�������߂�

	if (type == 0)
	{
		//�����w�肵���l�ȓ���������true
		if (difference >= diff)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	if (type == 1)
	{
		//�����w�肵���l�ȏゾ������true
		if (difference <= diff)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}


float LibMath::getFloatPI()
{
	return (float)M_PI;
	//return 0;
}

double LibMath::getDoublePI()
{
	return (double)M_PI;
	//return 0;
}

#pragma region �x�N�g��

float LibMath::calcDistance3D(Vector3 pos1, Vector3 pos2)
{
	return sqrt
	(
		(pos1.x - pos2.x) * (pos1.x - pos2.x) +
		(pos1.y - pos2.y) * (pos1.y - pos2.y) +
		(pos1.z - pos2.z) * (pos1.z - pos2.z)
	);
}

Vector3 LibMath::otherVector(Vector3 myPos, Vector3 otherPos)
{
	Vector3 vec;
	vec.x = (otherPos.x - myPos.x);
	vec.y = (otherPos.y - myPos.y);
	vec.z = (otherPos.z - myPos.z);
	float size = sqrt(vec.x *vec.x + vec.y *vec.y + vec.z *vec.z);
	return{ vec.x / size ,vec.y / size  ,vec.z / size };
}


float LibMath::twoVector2Angle(Vector2 v1, Vector2 v2)
{

	float f = Vector2::dot(v1, v2);
	f = std::acos(f);
	f = angleConversion(1, f);

	Vector3 v = Vector3::cross({ v1.x,v1.y,0 }, { v2.x,v2.y,0 });
	if (v.z < 0)f = 360 - f;

	return f;
}

float LibMath::vecto2rToAngle(Vector2 v)
{

	float f = twoVector2Angle({ 1,0 }, v);
	return f;
}


Vector2 LibMath::angleToVector2(float angle)
{
	DirectX::XMMATRIX vMat = DirectX::XMMatrixIdentity();
	vMat.r[3].m128_f32[0] = 1.0f;

	DirectX::XMMATRIX aMat = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angle));
	vMat *= aMat;
	return { vMat.r[3].m128_f32[0] , vMat.r[3].m128_f32[1] };

}


Vector2 LibMath::rotateVector2(Vector2 v, float angle)
{
	float vAngle = vecto2rToAngle(v);
	return angleToVector2(vAngle + angle);
}

#pragma endregion


#pragma region �����蔻��
#pragma region 2D

bool LibMath::rectCollision(Vector2 start1, Vector2 end1, Vector2 start2, Vector2 end2)//�l�p����
{
	if (end2.x + start2.x > start1.x &&
		start2.x < end1.x + start1.x &&
		start2.y < end1.y + start1.y &&
		end2.y + start2.y > start1.y)
	{
		return true;
	}
	return false;
}

bool LibMath::circleCollision(Vector2 centerPosition1, float r1, Vector2 centerPosition2, float r2)//�~����
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


	//���_���狅�̋���
	float sphereDist = Vector3::dot(spherePos, normal);

	float dict = sphereDist - planeDist;
	if (fabsf(dict) > r)return false;

	if (hitPos) *hitPos = normal * -dict + spherePos;
	return true;
}

bool LibMath::sphereAndTryangleCorrision
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
	//�ŋߐړ_
	Vector3 closest = { 0,0,0 };
	//����
	float dot1, dot2;
	//�����G���A
	int area = 0;

#pragma region �̈�`�F�b�N1

	if (area == 0)
	{
		//tryPos1�AtryPos2�̃x�N�g���ƁAtryPos1�AspherePos�̃x�N�g���̓��ς����߂�
		dot1 = Vector3::dot(triPos2 - triPos1, spherePos - triPos1);

		//tryPos1�AtryPos3�̃x�N�g���ƁAtryPos1�AspherePos�̃x�N�g���̓��ς����߂�
		dot2 = Vector3::dot(triPos3 - triPos1, spherePos - triPos1);

		//dot1��dot2�̗������}�C�i�X�Ȃ�A�̈�1�ɂ���
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 1;
			closest = triPos1;
		}
	}
#pragma endregion

#pragma region �̈�`�F�b�N2

	if (area == 0)
	{
		dot1 = Vector3::dot(triPos1 - triPos2, spherePos - triPos2);

		dot2 = Vector3::dot(triPos3 - triPos2, spherePos - triPos2);

		//dot1��dot2�̗������}�C�i�X�Ȃ�A�̈�1�ɂ���
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 2;
			closest = triPos2;
		}
	}
#pragma endregion

#pragma region �̈�`�F�b�N4

	if (area == 0)
	{
		dot1 = Vector3::dot(triPos2 - triPos3, spherePos - triPos3);

		dot2 = Vector3::dot(triPos1 - triPos3, spherePos - triPos3);

		//dot1��dot2�̗������}�C�i�X�Ȃ�A�̈�1�ɂ���
		if (dot1 <= 0 && dot2 <= 0)
		{
			area = 4;
			closest = triPos3;
		}
	}
#pragma endregion


#pragma region �̈�`�F�b�N3

	if (area == 0)
	{
		dot1 = Vector3::dot(triPos2 - triPos1, spherePos - triPos1);
		dot2 = Vector3::dot(triPos2 - triPos1, spherePos - triPos2);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3::dot(triPos2 - triPos1, spherePos - triPos1) * Vector3::dot(triPos3 - triPos1, spherePos - triPos2) -
				Vector3::dot(triPos2 - triPos1, spherePos - triPos2) * Vector3::dot(triPos3 - triPos1, spherePos - triPos1) <= 0)
			{
				area = 3;
				float num =
					Vector3::dot(triPos2 - triPos1, spherePos - triPos1) /
					(Vector3::dot(triPos2 - triPos1, spherePos - triPos1) - Vector3::dot(triPos2 - triPos1, spherePos - triPos2));
				closest.x = triPos1.x + num * (triPos2.x - triPos1.x);
				closest.y = triPos1.y + num * (triPos2.y - triPos1.y);
				closest.z = triPos1.z + num * (triPos2.z - triPos1.z);
			}


		}
	}
#pragma endregion

#pragma region �̈�`�F�b�N5
	if (area == 0)
	{
		dot1 = Vector3::dot(triPos3 - triPos2, spherePos - triPos2);
		dot2 = Vector3::dot(triPos3 - triPos2, spherePos - triPos3);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3::dot(triPos3 - triPos2, spherePos - triPos2) * Vector3::dot(triPos1 - triPos2, spherePos - triPos3) -
				Vector3::dot(triPos3 - triPos2, spherePos - triPos3) * Vector3::dot(triPos1 - triPos2, spherePos - triPos2) <= 0)
			{

				area = 5;

				float num =
					Vector3::dot(triPos3 - triPos2, spherePos - triPos2) /
					(Vector3::dot(triPos3 - triPos2, spherePos - triPos2) - Vector3::dot(triPos3 - triPos2, spherePos - triPos3));
				closest.x = triPos2.x + num * (triPos3.x - triPos2.x);
				closest.y = triPos2.y + num * (triPos3.y - triPos2.y);
				closest.z = triPos2.z + num * (triPos3.z - triPos2.z);

			}
		}
	}
#pragma endregion

#pragma region �̈�`�F�b�N6
	if (area == 0)
	{
		dot1 = Vector3::dot(triPos1 - triPos3, spherePos - triPos3);
		dot2 = Vector3::dot(triPos1 - triPos3, spherePos - triPos1);

		if (dot1 > 0 && dot2 <= 0)
		{
			if (Vector3::dot(triPos1 - triPos3, spherePos - triPos3) * Vector3::dot(triPos2 - triPos3, spherePos - triPos1) -
				Vector3::dot(triPos1 - triPos3, spherePos - triPos1) * Vector3::dot(triPos2 - triPos3, spherePos - triPos3) <= 0)
			{
				area = 6;

				float num =
					Vector3::dot(triPos1 - triPos3, spherePos - triPos3) /
					(Vector3::dot(triPos1 - triPos3, spherePos - triPos3) - Vector3::dot(triPos1 - triPos3, spherePos - triPos1));
				closest.x = triPos3.x + num * (triPos1.x - triPos3.x);
				closest.y = triPos3.y + num * (triPos1.y - triPos3.y);
				closest.z = triPos3.z + num * (triPos1.z - triPos3.z);

			}


		}
	}
#pragma endregion


#pragma region �̈�`�F�b�N7
	if (area == 0)
	{
		float va =
			Vector3::dot(triPos2 - triPos1, spherePos - triPos2) * Vector3::dot(triPos3 - triPos1, spherePos - triPos3) -
			Vector3::dot(triPos2 - triPos1, spherePos - triPos3) * Vector3::dot(triPos3 - triPos1, spherePos - triPos2);

		float vb =
			Vector3::dot(triPos2 - triPos1, spherePos - triPos3) * Vector3::dot(triPos3 - triPos1, spherePos - triPos1) -
			Vector3::dot(triPos2 - triPos1, spherePos - triPos1) * Vector3::dot(triPos3 - triPos1, spherePos - triPos3);

		float vc =
			Vector3::dot(triPos2 - triPos1, spherePos - triPos1) * Vector3::dot(triPos3 - triPos1, spherePos - triPos2) -
			Vector3::dot(triPos2 - triPos1, spherePos - triPos2) * Vector3::dot(triPos3 - triPos1, spherePos - triPos1);

		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		closest.x = triPos1.x + (triPos2.x - triPos1.x) * v + (triPos3.x - triPos1.x) * w;
		closest.y = triPos1.y + (triPos2.y - triPos1.y) * v + (triPos3.y - triPos1.y) * w;
		closest.z = triPos1.z + (triPos2.z - triPos1.z) * v + (triPos3.z - triPos1.z) * w;

		area = 7;
	}
#pragma endregion


	//�Őڋߓ_ - ���̒��S
	Vector3 v = closest - spherePos;
	float f = Vector3::dot(v, v);
	if (f > r * r) return false;

	if (hitPos)*hitPos = closest;
	return true;

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

	//�����|���ƕ��s�ł͂Ȃ����𒲂ׂ�(���s��������return)
	if (Vector3::dot(v1, normal) * Vector3::dot(v2, normal) > 0)
		return false;


	//�����Ƀ|���S���͈͓̔����ǂ����̏���������

	//���̒[ - �|���S���̊p
	v1 = linePos1 - vertexPoint[0];
	v2 = linePos2 - vertexPoint[0];
	/*v1 = Vector3::normalize(v1);
	v2 = Vector3::normalize(v2);*/


	//��������������?

	////���̒[����|���܂ł̋��������߂�
	//float d = Vector3::dot(normal,pointPos);
	//float kyori1;//���ʂ���AlinePos1�܂ł̋���
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
	if (normal.x != 0)kyori1 += abs(Vector3::dot(normal, vec1)) / abs(normal.x);
	if (normal.y != 0)kyori1 += abs(Vector3::dot(normal, vec1)) / abs(normal.y);
	if (normal.z != 0)kyori1 += abs(Vector3::dot(normal, vec1)) / abs(normal.z);
	float kyori2 = 0;
	if (normal.x != 0)kyori2 += abs(Vector3::dot(normal, vec2)) / abs(normal.x);
	if (normal.y != 0)	kyori2 += abs(Vector3::dot(normal, vec2)) / abs(normal.y);
	if (normal.z != 0)	kyori2 += abs(Vector3::dot(normal, vec2)) / abs(normal.z);

	/*float kyori1 = abs(Vector3::dot(normal, linePos1)) / abs(normal.x);
	float kyori2 = abs(Vector3::dot(normal, linePos2)) / abs(normal.x);*/

	//������
	float a;
	a = kyori1 / (kyori1 + kyori2);


	//������������

	//���ƃ|���S�����������Ă���ꏊ�𒲂ׂ�
	Vector3 crossVector;//�|���S���̊p���瓖�����Ă�ꏊ�̍��W�ւ̃x�N�g��
	crossVector.x = (1 - a) * v1.x + a * v2.x;
	crossVector.y = (1 - a) * v1.y + a * v2.y;
	crossVector.z = (1 - a) * v1.z + a * v2.z;
	Vector3 crossPos = vertexPoint[0] + crossVector;
	//crossPos.y *= - 1;



	//�O�p�`1�ڂ̔���
	Vector3 normal1;
	Library::calculationNormal(vertexPoint[0], vertexPoint[1], crossPos, normal1, normal1, normal1);
	Vector3 normal2;
	Library::calculationNormal(vertexPoint[1], vertexPoint[2], crossPos, normal2, normal2, normal2);
	Vector3 normal3;
	Library::calculationNormal(vertexPoint[2], vertexPoint[0], crossPos, normal3, normal3, normal3);

	//�|���Ɩ@�������������ׂ�
	bool equal1 = false;//�|���Ɩ@�����������ǂ���

	//������������true
	if (difference(normal.x, normal1.x, 0.0001, 0) &&
		difference(normal.y, normal1.y, 0.0001, 0) &&
		difference(normal.z, normal1.z, 0.0001, 0) &&
		difference(normal.x, normal2.x, 0.0001, 0) &&
		difference(normal.y, normal2.y, 0.0001, 0) &&
		difference(normal.z, normal2.z, 0.0001, 0) &&
		difference(normal.x, normal3.x, 0.0001, 0) &&
		difference(normal.y, normal3.y, 0.0001, 0) &&
		difference(normal.z, normal3.z, 0.0001, 0))
	{
		equal1 = true;
	}


	//�O�p�`2�ڂ̔���
	Library::calculationNormal(vertexPoint[2], vertexPoint[1], crossPos, normal1, normal1, normal1);
	Library::calculationNormal(vertexPoint[1], vertexPoint[3], crossPos, normal2, normal2, normal2);
	Library::calculationNormal(vertexPoint[3], vertexPoint[2], crossPos, normal3, normal3, normal3);

	//�|���Ɩ@�������������ׂ�
	bool equal2 = false;//�|���Ɩ@�����������ǂ���

	//����(�덷0.0001�ȓ�)��������true
	if (difference(normal.x, normal1.x, 0.0001, 0) &&
		difference(normal.y, normal1.y, 0.0001, 0) &&
		difference(normal.z, normal1.z, 0.0001, 0) &&
		difference(normal.x, normal2.x, 0.0001, 0) &&
		difference(normal.y, normal2.y, 0.0001, 0) &&
		difference(normal.z, normal2.z, 0.0001, 0) &&
		difference(normal.x, normal3.x, 0.0001, 0) &&
		difference(normal.y, normal3.y, 0.0001, 0) &&
		difference(normal.z, normal3.z, 0.0001, 0))
	{
		equal2 = true;
	}

	//�ǂ��炩������(�|���̒�)��������if�̒���
	if (equal1 || equal2)
	{
		//�Փˈʒu��Ԃ�
		if (crossPosition) *crossPosition = crossPos;

		return true;
	}

	//�Փˈʒu�ƒ��S��������������q�b�g
	if (difference(crossPos.x, pointPos.x, 0.01, 0) &&
		difference(crossPos.y, pointPos.y, 0.01, 0) && 
		difference(crossPos.z, pointPos.z, 0.01, 0) )
	{
		//�Փˈʒu��Ԃ�
		if (crossPosition) *crossPosition = crossPos;

		return true;
	}

#ifdef _DEBUG
	//�Փˈʒu��Ԃ�
	if (crossPosition) *crossPosition = crossPos;
#endif // _DEBUG


	return false;

}


bool LibMath::layAndPlaneCollision
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

	float d1 = Vector3::dot(normal, layDirection);

	if (d1 > -epsilon) return false;

	float d2 = Vector3::dot(normal, layStartPos);

	float dist = d2 - planeDistance;

	float t = dist / -d1;

	if (t < 0)return false;

	if (distance)*distance = t;
	if (crossPos)*crossPos = layStartPos + layDirection * t;

	return true;
}

bool LibMath::layAndTryangleCollision
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
	planeDistance = Vector3::dot(normal, triPos1);

	if (!layAndPlaneCollision(layStartPos, layDirection, planeNormal, planeDistance, distance, &interPlane))return false;



	//��������
	const float epsilon = 1.0e-5f;
	Vector3 m;

	Vector3 pt_p0 = triPos1 - interPlane;
	Vector3 p0_p1 = triPos2 - triPos1;
	m = Vector3::cross(pt_p0, p0_p1);
	if (Vector3::dot(m, normal) < -epsilon)return false;


	Vector3 pt_p1 = triPos2 - interPlane;
	Vector3 p1_p2 = triPos3 - triPos2;
	m = Vector3::cross(pt_p1, p1_p2);
	if (Vector3::dot(m, normal) < -epsilon)return false;


	Vector3 pt_p2 = triPos3 - interPlane;
	Vector3 p2_p0 = triPos1 - triPos3;
	m = Vector3::cross(pt_p2, p2_p0);
	if (Vector3::dot(m, normal) < -epsilon)return false;

	if (crossPos)*crossPos = interPlane;
	return true;

}

bool LibMath::layAndSphereCollision
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
	float b = Vector3::dot(m, layDirection);
	float c = Vector3::dot(m, m) - r * r;
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

