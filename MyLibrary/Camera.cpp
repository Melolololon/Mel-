#include "Camera.h"
#include"Library.h"
#include"LibMath.h"

std::unordered_map<std::string, std::unique_ptr<Camera>>Camera::pCameras;
UINT Camera::createCount = 0;
std::string Camera::mainCameraName = "";

void Camera::Create(const std::string& name)
{
	std::string key = name;
	if (key == "")key = "Camera_" + std::to_string(createCount);
	createCount++;

	pCameras.emplace(key, std::make_unique<Camera>());

	if (mainCameraName == "")mainCameraName = name;

}

void Camera::Delete(const std::string& name)
{
	pCameras.erase(name);
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	//�x�N�g�����J�����̊p�x�ɉ����ĉ�]������֐�
	auto RotateVectorCameraAngle = [&](Vector3& vector)
	{
		vector = LibMath::RotateVector3(vector, Vector3(0, 0, 1), angle.z);
		vector = LibMath::RotateVector3(vector, Vector3(1, 0, 0), angle.x);
		vector = LibMath::RotateVector3(vector, Vector3(0, 1, 0), angle.y);
	};

	Vector3 target = Vector3(0, 0, 1);
	RotateVectorCameraAngle(target);
	target += position;

	Vector3 upVector = Vector3(0, 1, 0);
	RotateVectorCameraAngle(upVector);


	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&position.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&target.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&upVector.ToXMFLOAT3())
	);

}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{

	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //��p
		(float)Library::GetWindowWidth() / (float)Library::GetWindowHeight(),//�A�X�y�N�g��
		nearNum, farNum//�`��͈�
	);
}

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()const
{
	return GetViewMatrix() * GetProjectionMatrix();
}
