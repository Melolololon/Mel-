#include"CollisionType.h"
#include"LibMath.h"

MelLib::Value2<MelLib::Vector3> MelLib::CapsuleData::CalcCapsuleLineSegmentPos()const
{
	//ベクトルを回転
	Vector3 rotateVector(Quaternion::GetZXYRotateQuaternion(Vector3(0, 1, 0), angle).ToVector3());

	//回転させたベクトルの方向にlength / 2だけ移動
	Vector3 rotateMovePos(LibMath::FloatDistanceMoveVector3(0, rotateVector, length / 2));
	Value2<Vector3>capsuleLineSegmentPos(rotateMovePos, -rotateMovePos);

	//平行移動
	capsuleLineSegmentPos.v1 += position;
	capsuleLineSegmentPos.v2 += position;

	return capsuleLineSegmentPos;
}
