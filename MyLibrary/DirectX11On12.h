#pragma once
#include<d3d12.h>
#include<d3d11.h>
#include<d3d11on12.h>
#pragma comment(lib,"D3D11.lib")

#include<wrl.h>
using namespace Microsoft::WRL;

//11on12を使用したD2D
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/d2d-using-d3d11on12

//

//D3D11on12クラス
class DirectX11On12 final
{
private:

	DirectX11On12(){}
	~DirectX11On12(){}

	//11のデバイス
	ComPtr<ID3D11Device>device11;
	ComPtr<ID3D11DeviceContext>context;
	ComPtr<ID3D11On12Device>device11On12;
public:
	DirectX11On12(DirectX11On12& d) = delete;
	DirectX11On12& operator= (DirectX11On12& d) = delete;
	static DirectX11On12* GetInstance();

	void Initialize(ID3D12Device* dev12,ID3D12CommandQueue* queue12);

};

