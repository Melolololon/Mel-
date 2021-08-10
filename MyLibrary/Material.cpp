#include "Material.h"
#include"CreateBuffer.h"
#include<d3dx12.h>

ID3D12Device* MelLib::Material::device;

void MelLib::Material::CreateBufferAndDescriptorHeap(const size_t& mtlByte)
{

	//定数バッファ作成
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		mtlByte,
		&materialBuffer
	);
	//定数バッファ作成
	CreateBuffer::GetInstance()->CreateConstBuffer
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		sizeof(DirectX::XMFLOAT4),
		&colorBuffer
	);

	//ディスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 3;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureHeap));
}

void MelLib::Material::MapMaterialData(void** pData, const MaterialConstBufferType type)
{
	switch (type)
	{
	case MaterialConstBufferType::COLOR:
		colorBuffer->Map(0, nullptr, pData);
		break;
	case MaterialConstBufferType::MATERIAL_DATA:
		materialBuffer->Map(0, nullptr, pData);
		break;
	}
}

void MelLib::Material::UnmapMaterialData(const MaterialConstBufferType type)
{
	switch (type)
	{
	case MaterialConstBufferType::COLOR:
		colorBuffer->Unmap(0, nullptr);
		break;
	case MaterialConstBufferType::MATERIAL_DATA:
		materialBuffer->Unmap(0, nullptr);
		break;
	}
}

ID3D12Resource* MelLib::Material::GetPConstBuffer(const MaterialConstBufferType type) const
{
	switch (type)
	{
	case MaterialConstBufferType::COLOR:
		return colorBuffer.Get();
		break;

	case MaterialConstBufferType::MATERIAL_DATA:
		return materialBuffer.Get();
	}
}

void MelLib::Material::SetColor(const Color& color)
{ 
	this->color = color; 
	DirectX::XMFLOAT4* col;
	MapMaterialData((void**)&col, MaterialConstBufferType::COLOR);
	*col = DirectX::XMFLOAT4((float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255);
	UnmapMaterialData(MaterialConstBufferType::COLOR);
}

void MelLib::Material::SetTexture(Texture* pTex)
{ 
	
	pTexture = pTex; 
	if (pTex) 
	{
		CreateBuffer::GetInstance()->CreateShaderResourceView
		(
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				textureHeap->GetCPUDescriptorHandleForHeapStart(),
				TEXTURE_HANDLE_NUM,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			pTex->GetTextureBuffer()
		);


		DirectX::XMFLOAT4* col;
		MapMaterialData((void**)&col, MaterialConstBufferType::COLOR);
		*col = DirectX::XMFLOAT4(0, 0, 0, 0);
		UnmapMaterialData(MaterialConstBufferType::COLOR);
	}
	else
	{
		DirectX::XMFLOAT4* col;
		MapMaterialData((void**)&col, MaterialConstBufferType::COLOR);
		*col = DirectX::XMFLOAT4((float)color.r / 255, (float)color.g / 255, (float)color.b / 255, (float)color.a / 255);
		UnmapMaterialData(MaterialConstBufferType::COLOR);
	}



}

void MelLib::Material::SetNormalMapTexture(Texture* pTex)
{
	pNormalMapTexture = pTex; 
	if (!pTex)return;
	CreateBuffer::GetInstance()->CreateShaderResourceView
	(
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			textureHeap->GetCPUDescriptorHandleForHeapStart(),
			NORMAL_MAP_HANDLE_NUM,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		pTex->GetTextureBuffer()
	);
}

void MelLib::Material::SetTexture3D(Texture3D* pTex)
{
	pTexture3D = pTex;
	if (!pTex)return;
	CreateBuffer::GetInstance()->CreateShaderResourceView
	(
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			textureHeap->GetCPUDescriptorHandleForHeapStart(),
			TEXTURE_3D_HANDLE_NUM,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		pTex->GetTextureBuffer()
	);
}



void MelLib::ADSAMaterial::Create()
{
	CreateBufferAndDescriptorHeap(sizeof(ADSAMaterialData));
}

void MelLib::ADSAMaterial::Map()
{
	ADSAMaterialData* mtl = nullptr;
	MapMaterialData((void**)&mtl,MaterialConstBufferType::MATERIAL_DATA);
	*mtl = materialData;
	UnmapMaterialData(MaterialConstBufferType::MATERIAL_DATA);

}

void MelLib::ADSAMaterial::SetMaterialData(const ADSAMaterialData& data)
{ 
	materialData = data;
	Map();
}

void MelLib::PBRMaterial::Map()
{
	PBRMaterialData* mtl = nullptr;
	MapMaterialData((void**)&mtl, MaterialConstBufferType::MATERIAL_DATA);
	*mtl = materialData;
	UnmapMaterialData(MaterialConstBufferType::MATERIAL_DATA);
}

void MelLib::PBRMaterial::Create()
{
	CreateBufferAndDescriptorHeap(sizeof(PBRMaterial));
}

void MelLib::PBRMaterial::SetMaterialData(const PBRMaterialData& data)
{
	materialData = data;
	Map();
}
