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

void MelLib::Segment3DData::SetAngle(const Vector3& angle)
{
	this->angle = angle;
	CalcRotatePosition();
}
