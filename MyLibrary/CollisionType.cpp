#include"CollisionType.h"
#include"LibMath.h"

using namespace MelLib;

void  MelLib::Segment3DData::CalcRotatePosition()
{
	Vector3 centerPos = LibMath::CalcCenterPosition3D(position.v1, position.v2);;
	
	//中心が原点にいるときの座標で回転
	rotatePosition = Value2<Vector3>
		(
			Quaternion::GetZXYRotateQuaternion(position.v1 - centerPos, Vector3(angle.x, angle.y, angle.z)).ToVector3(),
			Quaternion::GetZXYRotateQuaternion(position.v2 - centerPos, Vector3(angle.x, angle.y, angle.z)).ToVector3()
		);

	//引いた分戻す
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

	
	//前との差分を求めて、座標を移動させる
	//差の大きさ分、中心方向の逆にlength分移動(lengthが-だったら中心に近づく)
	//lengthだとマイナス入らない

	//差(マイナスあり)を求める
	Vector2 curSubPre = size - preSize;
	Vector3 pos = position;

	/// <summary>
	/// サイズを適応させるラムダ式
	/// </summary>
	/// <param name="size"></param>
	auto calcMovePos = [&curSubPre,&pos](const Vector3& dirPos)
	{
		Vector3 posToDirPos = LibMath::OtherVector(pos, dirPos);
		Vector3 returnPos;

		//差のX分移動
		returnPos = LibMath::FloatDistanceMoveVector3(dirPos, posToDirPos.x, curSubPre.x);
		//差のY分移動
		returnPos = LibMath::FloatDistanceMoveVector3(returnPos, posToDirPos.y, curSubPre.y);

		return returnPos;
	};

	//サイズ分拡縮
	leftDownPos = calcMovePos(leftDownPos);
	leftUpPos = calcMovePos(leftUpPos);
	rightDownPos = calcMovePos(rightDownPos);
	rightUpPos = calcMovePos(rightUpPos);

}

void MelLib::BoardData::SetAngle(const Vector3& angle)
{
	this->angle = angle;

	//回転
	normal = Quaternion::GetZXYRotateQuaternion(normal, angle).ToVector3();
	leftDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	leftUpPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	rightDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	rightUpPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();


}
