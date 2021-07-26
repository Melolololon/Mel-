#include "DirectX11On12.h"

DirectX11On12* DirectX11On12::GetInstance()
{
	static DirectX11On12 instance;
	return &instance;
}

void DirectX11On12::Initialize(ID3D12Device* dev12, ID3D12CommandQueue* queue12)
{
	UINT flag =
		D3D11_CREATE_DEVICE_DEBUG
		|| D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	auto result = D3D11On12CreateDevice
	(
		dev12,
		flag,
		nullptr,//nullptrにするとD3D12の方と同じレベルになる
		0,//上の引数の配列数(今回はここで指定しないから0)
		reinterpret_cast<IUnknown**>(&queue12),//D3D12のキューのポインタをキャストしたものでいい?
		1,//キューの数
		0,//D3D12デバイスのノード
		&device11,
		&context,
		nullptr//選ばれたレベルを取得する場合ポインタを渡す
	);

	//これ調べる
	device11.As(&device11On12);

}
