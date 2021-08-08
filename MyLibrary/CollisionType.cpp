#include"CollisionType.h"
#include"LibMath.h"

MelLib::Value2<MelLib::Vector3> MelLib::CapsuleData::CalcCapsuleLineSegmentPos()const
{
	//�x�N�g������]
	Vector3 rotateVector(Quaternion::GetZXYRotateQuaternion(Vector3(0, 1, 0), angle).ToVector3());

	//��]�������x�N�g���̕�����length / 2�����ړ�
	Vector3 rotateMovePos(LibMath::FloatDistanceMoveVector3(0, rotateVector, length / 2));
	Value2<Vector3>capsuleLineSegmentPos(rotateMovePos, -rotateMovePos);

	//���s�ړ�
	capsuleLineSegmentPos.v1 += position;
	capsuleLineSegmentPos.v2 += position;

	return capsuleLineSegmentPos;
}
