#include "Camera.h"
#include"Library.h"
#include"LibMath.h"

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()
{
	//eyeからtargetへのベクトル
	Vector3 eyeToTarget = Vector3(0, 0, 1);
	Vector3 upVector = Vector3(0, 1, 0);

	//ベクトルをカメラの角度に応じて回転させる関数
	auto RotateVectorCameraAngle = [&](Vector3& vector)
	{
		vector = LibMath::RotateVector3(vector, Vector3(0, 0, 1), angle.z);
		vector = LibMath::RotateVector3(vector, Vector3(1, 0, 0), angle.x);
		vector = LibMath::RotateVector3(vector, Vector3(0, 1, 0), angle.y);
	};

	RotateVectorCameraAngle(eyeToTarget);
	RotateVectorCameraAngle(upVector);


	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&position.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&eyeToTarget.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&upVector.ToXMFLOAT3())
	);


	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //画角
		Library::GetWindowWidth() / Library::GetWindowHeight(),//アスペクト比
		nearNum, farNum//描画範囲
	);

	return matView * mapProjection;
}
