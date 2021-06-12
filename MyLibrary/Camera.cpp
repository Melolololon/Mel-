#include "Camera.h"
#include"Library.h"

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()
{

	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //��p
		Library::GetWindowWidth() / Library::GetWindowHeight(),//�A�X�y�N�g��
		nearNum, farNum//�`��͈�
	);

	//return DirectX::XMMATRIX();
}
