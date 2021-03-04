#pragma once
#include<DirectXMath.h>
#include"DirectXStruct.h"
class CreateCamera
{
private:
	int windowWidth;
	int windowHeight;


public:
	CreateCamera(int winWidth,int winHeight);
	~CreateCamera();
	
	DirectX::XMMATRIX get2DCameraMatrix();
	DirectX::XMMATRIX get3DCameraMatrix(CameraData cameraData);

	/// <summary>
	/// ��]�s���K���������J�������W��Ԃ��܂�
	/// </summary>
	/// <param name="cameraData"></param>
	/// <returns></returns>
	void get3DCameraPosition(CameraData cameraData,DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& target);

	DirectX::XMMATRIX getViewMatrix(CameraData cameraData);
	DirectX::XMMATRIX getProjectionMatrix(CameraData cameraData);
};

