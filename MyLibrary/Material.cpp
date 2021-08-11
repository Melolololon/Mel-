#include "Material.h"
#include"CreateBuffer.h"
#include<d3dx12.h>

ID3D12Device* MelLib::Material::device;

MelLib::Material::~Material()
{
	if (textureType == SetTextureType::LOAD)delete pTexture;
	if (normalMapTextureType == SetTextureType::LOAD)delete pNormalMapTexture;
	if (texture3DType == SetTextureType::LOAD)delete pTexture3D;
}


void MelLib::Material::MapColorBuffer(const Color& color)
{
	DirectX::XMFLOAT4* col;
	colorBuffer->Map(0, nullptr, (void**)&col);
	*col = DirectX::XMFLOAT4((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f);
	colorBuffer->Unmap(0, nullptr);

}

void MelLib::Material::SetOrLoadTextureProcess()
{
	if (pTexture)
	{
		CreateBuffer::GetInstance()->CreateShaderResourceView
		(
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				textureHeap->GetCPUDescriptorHandleForHeapStart(),
				TEXTURE_HANDLE_NUM,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			pTexture->GetTextureBuffer()
		);


		DirectX::XMFLOAT4* col;

		MapColorBuffer(Color(0, 0, 0, 0));
	}
	else
	{
		textureType = SetTextureType::NONE;

		MapColorBuffer(color);
	}


}

void MelLib::Material::CreateInitialize(const size_t& mtlByte)
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
	MapColorBuffer(color);
	

	//ディスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 3;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureHeap));

	
}

void MelLib::Material::MapMaterialData(void** pData)
{
	materialBuffer->Map(0, nullptr, pData);
}

void MelLib::Material::UnmapMaterialData()
{
	materialBuffer->Unmap(0, nullptr);
	
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
	MapColorBuffer(color);
}

void MelLib::Material::SetTexture(Texture* pTex)
{ 
	//Load関数で読み込んだテクスチャがセットされてたら解放
	if(textureType == SetTextureType::LOAD)
	{
		delete pTexture;
	}

	pTexture = pTex; 
	SetOrLoadTextureProcess();
	if(pTexture)textureType = SetTextureType::SET;
	else textureType = SetTextureType::NONE;

}

void MelLib::Material::SetNormalMapTexture(Texture* pTex)
{
	if (normalMapTextureType == SetTextureType::LOAD)
	{
		delete pNormalMapTexture;
	}

	pNormalMapTexture = pTex; 
	if (!pTex)
	{
		normalMapTextureType = SetTextureType::NONE;
		return;
	}

	normalMapTextureType = SetTextureType::SET;
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
	if (texture3DType == SetTextureType::LOAD)
	{
		delete pTexture3D;
	}

	pTexture3D = pTex;
	if (!pTex)
	{
		texture3DType = SetTextureType::NONE;
		return;
	}
	
	texture3DType = SetTextureType::SET;
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

void MelLib::Material::SetLoadTexture(const std::string& path)
{
	Texture* preTexture = pTexture;
	
	pTexture = new Texture();
	bool result = pTexture->LoadModelTexture(path);

	//読み込み失敗したら元に戻す
	if(!result)
	{
		delete pTexture;
		pTexture = preTexture;
		return;
	}
	
	SetOrLoadTextureProcess();
	textureType = SetTextureType::LOAD;
}



#pragma region ADSA

void MelLib::ADSAMaterial::Create(const DrawData& drawData)
{
	CreateInitialize(sizeof(ADSAMaterialData));
	Map();

	//パイプライン作成
	ShaderDataSet set =
	{
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"LIB","","" },
		{ L"LIB","","" },
		{ L"../MyLibrary/FbxGeometryShader.hlsl","main","gs_5_0" },
		{ L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" }
	};

	pipelineState.CreatePipeline
	(
		drawData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);

}

void MelLib::ADSAMaterial::Map()
{
	ADSAMaterialData* mtl = nullptr;
	MapMaterialData((void**)&mtl);
	*mtl = materialData;
	UnmapMaterialData();

}

void MelLib::ADSAMaterial::SetMaterialData(const ADSAMaterialData& data)
{ 
	materialData = data;
	Map();
}

#pragma endregion


#pragma region PBR


void MelLib::PBRMaterial::Map()
{
	PBRMaterialData* mtl = nullptr;
	MapMaterialData((void**)&mtl);
	*mtl = materialData;
	UnmapMaterialData();
}

void MelLib::PBRMaterial::Create(const DrawData& drawData)
{
	CreateInitialize(sizeof(PBRMaterial)); 
	Map();

	//ここPBRのシェーダー作ってセットするようにする
	//パイプライン作成
	ShaderDataSet set =
	{
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"LIB","","" },
		{ L"LIB","","" },
		{ L"../MyLibrary/FbxGeometryShader.hlsl","main","gs_5_0" },
		{ L"../MyLibrary/PBRPixelShader.hlsl","main","ps_5_0" }
	};

	pipelineState.CreatePipeline
	(
		drawData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);
}

void MelLib::PBRMaterial::SetMaterialData(const PBRMaterialData& data)
{
	materialData = data;
	Map();
}

#pragma endregion