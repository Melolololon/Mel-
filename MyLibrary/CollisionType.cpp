#include"CollisionType.h"
#include"LibMath.h"

using namespace MelLib;

void  MelLib::Segment3DData::CalcRotatePosition()
{
	Vector3 centerPos = LibMath::CalcCenterPosition3D(position.v1, position.v2);;
	
	//���S�����_�ɂ���Ƃ��̍��W�ŉ�]
	rotatePosition = Value2<Vector3>
		(
			Quaternion::GetZXYRotateQuaternion(position.v1 - centerPos, Vector3(angle.x, angle.y, angle.z)).ToVector3(),
			Quaternion::GetZXYRotateQuaternion(position.v2 - centerPos, Vector3(angle.x, angle.y, angle.z)).ToVector3()
		);

	//���������߂�
	rotatePosition.v1 += centerPos;
	rotatePosition.v2 += centerPos;

}

void MelLib::Segment3DData::SetPosition(const Value2<Vector3>& pos)
{
	Value2<Vector3> prePos = position;
	position = pos;


	Value2<Vector3>posSubPre = pos - prePos;
	rotatePosition += posSubPre;
}

void MelLib::Segment3DData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;
	this->angle = angle;
	CalcRotatePosition();
}




void MelLib::BoardData::SetPosition(const Vector3& pos)
{
	Vector3 prePos = position;
	position = pos;

	Vector3 posSubPre = pos - prePos;

	leftDownPos += posSubPre;
	leftUpPos += posSubPre;
	rightUpPos += posSubPre;
	rightDownPos += posSubPre;
}

void MelLib::BoardData::SetSize(const Vector2& size)
{
	Vector2 preSize = this->size;
	this->size = size;

	
	//�O�Ƃ̍��������߂āA���W���ړ�������
	//���̑傫�����A���S�����̋t��length���ړ�(length��-�������璆�S�ɋ߂Â�)
	//length���ƃ}�C�i�X����Ȃ�

	//��(�}�C�i�X����)�����߂�
	Vector2 curSubPre = size - preSize;
	Vector3 pos = position;

	/// <summary>
	/// �T�C�Y��K�������郉���_��
	/// </summary>
	/// <param name="size"></param>
	auto calcMovePos = [&curSubPre,&pos](const Vector3& dirPos)
	{
		Vector3 posToDirPos = LibMath::OtherVector(pos, dirPos);
		Vector3 returnPos;

		//����X���ړ�
		returnPos = LibMath::FloatDistanceMoveVector3(dirPos, posToDirPos.x, curSubPre.x);
		//����Y���ړ�
		returnPos = LibMath::FloatDistanceMoveVector3(returnPos, posToDirPos.y, curSubPre.y);

		return returnPos;
	};

	//�T�C�Y���g�k
	leftDownPos = calcMovePos(leftDownPos);
	leftUpPos = calcMovePos(leftUpPos);
	rightDownPos = calcMovePos(rightDownPos);
	rightUpPos = calcMovePos(rightUpPos);

}

void MelLib::BoardData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;
	this->angle = angle;

	//��]
	normal = Quaternion::GetZXYRotateQuaternion(normal, angle).ToVector3();
	leftDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	leftUpPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	rightDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	rightUpPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();


}
