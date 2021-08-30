#include"CollisionType.h"
#include"LibMath.h"
#include"Interpolation.h"

using namespace MelLib;

void  MelLib::Segment3DData::CalcRotatePosition()
{


	Vector3 v1ToV2 = Interpolation::Lerp(position.v1, position.v2, rotatePoint);

	//rotatePosition = Value2<Vector3>
	//	(
	//		Quaternion::GetZXYRotateQuaternion(position.v1 - v1ToV2, Vector3(angle.x, angle.y, angle.z)).ToVector3(),
	//		Quaternion::GetZXYRotateQuaternion(position.v2 - v1ToV2, Vector3(angle.x, angle.y, angle.z)).ToVector3()
	//	);


	rotatePosition.v1 = Quaternion::GetRotateQuaternion(position.v1 - v1ToV2, axisZVector, angle.z).ToVector3();
	rotatePosition.v1 = Quaternion::GetRotateQuaternion(rotatePosition.v1, axisXVector, angle.x).ToVector3();
	rotatePosition.v1 = Quaternion::GetRotateQuaternion(rotatePosition.v1, axisYVector, angle.y).ToVector3();


	rotatePosition.v2 = Quaternion::GetRotateQuaternion(position.v2 - v1ToV2, axisZVector, angle.z).ToVector3();
	rotatePosition.v2 = Quaternion::GetRotateQuaternion(rotatePosition.v2, axisXVector, angle.x).ToVector3();
	rotatePosition.v2 = Quaternion::GetRotateQuaternion(rotatePosition.v2, axisYVector, angle.y).ToVector3();

	//戻す
	rotatePosition.v1 += v1ToV2;
	rotatePosition.v2 += v1ToV2;
}

void MelLib::Segment3DData::SetPosition(const Value2<Vector3>& pos)
{
	Value2<Vector3> prePos = position;
	position = pos;

	if (position == prePos)return;
	CalcRotatePosition();
}

void MelLib::Segment3DData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;
	this->angle = angle;
	CalcRotatePosition();
}

void  MelLib::Segment3DData::SetRotatePoint(const float num)
{
	if (rotatePoint == num)return;
	rotatePoint = num;
	CalcRotatePosition();
}

void MelLib::Segment3DData::SetAxisAngle(const Vector3& angle)
{
	if (axisAngle == angle)return;
	axisAngle = angle;

	axisXVector = Quaternion::GetZXYRotateQuaternion(Vector3(-1, 0, 0), angle).ToVector3().Normalize();
	axisYVector = Quaternion::GetZXYRotateQuaternion(Vector3(0, -1, 0), angle).ToVector3().Normalize();
	axisZVector = Quaternion::GetZXYRotateQuaternion(Vector3(0, 0, -1), angle).ToVector3().Normalize();
}


void MelLib::BoardData::CalcRotateDirPosition()
{
	//ここ行列にしたほうがいい
	//行列使いまわしたほうが計算コスト少ない

	leftDownPos = Quaternion::GetZXYRotateQuaternion(leftDownPos, angle).ToVector3();
	leftUpPos = Quaternion::GetZXYRotateQuaternion(leftUpPos, angle).ToVector3();
	rightDownPos = Quaternion::GetZXYRotateQuaternion(rightDownPos, angle).ToVector3();
	rightUpPos = Quaternion::GetZXYRotateQuaternion(rightUpPos, angle).ToVector3();
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
	if (this->size == size)return;

	Vector2 preSize = this->size;
	this->size = size;


	//前との差分を求めて、座標を移動させる
	//差の大きさ分、中心方向の逆にlength分移動(lengthが-だったら中心に近づく)
	//lengthだとマイナス入らない


	//0だとベクトルを求められないので、仕方なく回転させる
	if (preSize == 0.0f)
	{
		leftDownPos = MelLib::Vector3(-size.x, -size.y, 0) / 2;
		leftUpPos = MelLib::Vector3(-size.x, size.y, 0) / 2;
		rightDownPos = MelLib::Vector3(size.x, -size.y, 0) / 2;
		rightUpPos = MelLib::Vector3(size.x, size.y, 0) / 2;

		//回転
		CalcRotateDirPosition();
	}
	else
	{
		//差(マイナスあり)を求める
		Vector2 curSubPre = size - preSize;
		Vector3 pos = position;

		/// <summary>
		/// サイズを適応させるラムダ式
		/// </summary>
		/// <param name="size"></param>
		auto calcMovePos = [&curSubPre, &pos](const Vector3& dirPos)
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
}

void MelLib::BoardData::SetAngle(const Vector3& angle)
{
	if (this->angle == angle)return;
	this->angle = angle;

	//回転
	normal = Quaternion::GetZXYRotateQuaternion(normal, angle).ToVector3();
	CalcRotateDirPosition();

}
