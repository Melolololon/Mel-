#include "Camera.h"
#include"Library.h"
#include"LibMath.h"

std::unordered_map<std::string, std::unique_ptr<Camera>>Camera::pCameras;
UINT Camera::createCount = 0;
std::string Camera::mainCameraName = "";

void Camera::CalcCameraData()
{	
	//�x�N�g�����J�����̊p�x�ɉ����ĉ�]������֐�
	auto RotateVectorCameraAngle = [&](Vector3& vector)
	{
		vector = LibMath::RotateVector3(vector, Vector3(0, 0, 1), angle.z);
		vector = LibMath::RotateVector3(vector, Vector3(1, 0, 0), angle.x);
		vector = LibMath::RotateVector3(vector, Vector3(0, 1, 0), angle.y);
	};


	//���̃A���S���Y���ŁA�J������90�x��](X��-�����Ɍ����Ă�)�Ƃ��ɁAFPS����TPS�؂�ւ���ƁA
	//�J�������v���C���[�̌��ɍs���āA�X�J�C�����̃J�����؂�ւ��݂����ɂȂ�
	if (cameraMode == CameraMode::CAMERA_MODE_FPS)
	{
		targetPosition = Vector3(0, 0, cameraToTargetDistance);
		RotateVectorCameraAngle(targetPosition);
		targetPosition += position;

		cameraPosition = position;
	}
	else
	{
		cameraPosition = Vector3(0, 0, -cameraToTargetDistance);
		RotateVectorCameraAngle(cameraPosition);
		cameraPosition += position;

		targetPosition = position;
	}

	upVector = Vector3(0, 1, 0);
	RotateVectorCameraAngle(upVector);
}

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

void Camera::SetPosition(const Vector3& position)
{
	this->position = position;

	CalcCameraData();
	//��
	DirectInput::SetViewMatrix(GetViewMatrix());
	DirectInput::SetProjectionMatrix(GetProjectionMatrix());

}

void Camera::SetAngle(const Vector3& angle)
{

	this->angle = angle;
	CalcCameraData();
	//��
	DirectInput::SetViewMatrix(GetViewMatrix());
	DirectInput::SetProjectionMatrix(GetProjectionMatrix());


}

void Camera::SetCameraToTargetDistance(const float distance)
{
	cameraToTargetDistance = distance;
	CalcCameraData();
}

void Camera::SetCameraMode(const CameraMode mode)
{
	cameraMode = mode;
	CalcCameraData();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{

	DirectX::XMMATRIX matView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat3(&cameraPosition.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&targetPosition.ToXMFLOAT3()),
		DirectX::XMLoadFloat3(&upVector.ToXMFLOAT3())
	);
	return matView;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{

	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //��p
		(float)Library::GetWindowWidth() / (float)Library::GetWindowHeight(),//�A�X�y�N�g��
		nearNum, farNum//�`��͈�
	);
	return mapProjection;
}

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()const
{
	return GetViewMatrix() * GetProjectionMatrix();
}
