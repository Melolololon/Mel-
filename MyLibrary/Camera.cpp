#include "Camera.h"
#include"Library.h"

DirectX::XMMATRIX Camera::GetViewAndProjectionMat()
{

	DirectX::XMMATRIX mapProjection = DirectX::XMMatrixPerspectiveFovLH
	(
		DirectX::XMConvertToRadians(fovY), //画角
		Library::GetWindowWidth() / Library::GetWindowHeight(),//アスペクト比
		nearNum, farNum//描画範囲
	);

	//return DirectX::XMMATRIX();
}
