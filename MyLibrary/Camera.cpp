#include "Camera.h"
#include"Library.h"
#include"LibMath.h"

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()
{
	//eye����target�ւ̃x�N�g��
	Vector3 eyeToTarget = Vector3(0, 0, 1);
	Vector3 upVector = Vector3(0, 1, 0);

	//�x�N�g�����J�����̊p�x�ɉ����ĉ�]������֐�
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
		DirectX::XMConvertToRadians(fovY), //��p
		Library::GetWindowWidth() / Library::GetWindowHeight(),//�A�X�y�N�g��
		nearNum, farNum//�`��͈�
	);

	return matView * mapProjection;
}
