#include "Material.h"
#include"CreateBuffer.h"
#include<d3dx12.h>

ID3D12Device* MelLib::Material::device;
ComPtr<ID3D12Resource>MelLib::Material::textureNoneTextureBuffer;

void MelLib::Material::MapColorBuffer(const Color& color,const size_t index)
{
	DirectX::XMFLOAT4* col;
	colorBuffers[index]->Map(0, nullptr, (void**)&col);
	*col = DirectX::XMFLOAT4((float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f);
	colorBuffers[index]->Unmap(0, nullptr);

}

void MelLib::Material::SetOrLoadTextureProcess()
{
	


}

void MelLib::Material::SetTextureMapBuffer(Texture* pTexture,const size_t index)
{
	if (pTexture)
	{
		if (!colorBuffers[index])
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

			MapColorBuffer(Color(0, 0, 0, 0), index);
		}
	}
	else
	{
		if (!colorBuffers[index])
		{
			CreateBuffer::GetInstance()->CreateShaderResourceView
			(
				CD3DX12_CPU_DESCRIPTOR_HANDLE
				(
					textureHeap->GetCPUDescriptorHandleForHeapStart(),
					TEXTURE_HANDLE_NUM,
					device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				),
				textureNoneTextureBuffer.Get()
			);

			MapColorBuffer(color, index);
		}

	}
}

void MelLib::Material::CreateConstBuffer()
{
	for (int i = 0; i < pTextures.size(); i++) 
	{
		if (!materialBuffers[i])continue;

		//定数バッファ作成
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			materialByte,
			&materialBuffers[i]
		);

		if (!colorBuffers[i])continue;
		//定数バッファ作成
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			sizeof(DirectX::XMFLOAT4),
			&colorBuffers[i]
		);
	}
}

void MelLib::Material::ResizeBuffer()
{
	if (pTextures.size() == materialBuffers.size())return;

	materialBuffers.resize(pTextures.size());
	colorBuffers.resize(pTextures.size());
}

void MelLib::Material::CreateInitialize(const size_t& mtlByte)
{
	materialByte = mtlByte;

	////定数バッファ作成
	//CreateBuffer::GetInstance()->CreateConstBuffer
	//(
	//	CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	mtlByte,
	//	&materialBuffer
	//);

	////定数バッファ作成
	//CreateBuffer::GetInstance()->CreateConstBuffer
	//(
	//	CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	sizeof(DirectX::XMFLOAT4),
	//	&colorBuffer
	//);
	//MapColorBuffer(color,0);
	

	//ディスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.NumDescriptors = 2;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	auto result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureHeap));

	//デフォルトはテクスチャセットしないので、テクスチャ未セット時のバッファのビューを作成
	CreateBuffer::GetInstance()->CreateShaderResourceView
	(
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			textureHeap->GetCPUDescriptorHandleForHeapStart(),
			TEXTURE_HANDLE_NUM,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		textureNoneTextureBuffer.Get()
	);
}

void MelLib::Material::MapMaterialData(void** pData)
{
	materialBuffer->Map(0, nullptr, pData);
}

void MelLib::Material::UnmapMaterialData()
{
	materialBuffer->Unmap(0, nullptr);
	
}

void MelLib::Material::Initialize(ID3D12Device* dev)
{ 
	device = dev;

	CreateBuffer::GetInstance()->CreateOneColorTextureBuffer
	(
		Color(0, 0, 0, 0),
		&textureNoneTextureBuffer
	);

}

ID3D12Resource* MelLib::Material::GetPConstBuffer(const MaterialConstBufferType type,const size_t index) const
{
	switch (type)
	{
	case MaterialConstBufferType::COLOR:
		return colorBuffers[index].Get();
		break;

	case MaterialConstBufferType::MATERIAL_DATA:
		return materialBuffers[index].Get();
	}
}

void MelLib::Material::SetColor(const Color& color)
{ 
	this->color = color; 
	MapColorBuffer(color,0);
}

void MelLib::Material::SetTexture(Texture* pTex)
{ 
	pTexture = pTex; 
	if (pTexture)
	{
		if (!colorBuffer) 
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

			MapColorBuffer(Color(0, 0, 0, 0),0);
		}
	}
	else
	{
		if (!colorBuffer)
		{
			CreateBuffer::GetInstance()->CreateShaderResourceView
			(
				CD3DX12_CPU_DESCRIPTOR_HANDLE
				(
					textureHeap->GetCPUDescriptorHandleForHeapStart(),
					TEXTURE_HANDLE_NUM,
					device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				),
				textureNoneTextureBuffer.Get()
			);

			MapColorBuffer(color, 0);
		}

	}
}

void MelLib::Material::SetTextures(Texture* pTex, size_t index)
{
	if (pTextures.size() <= index)
	{
		index = pTextures.size() - 1;
		pTextures.push_back(pTex); 

		ResizeBuffer();
		CreateConstBuffer();
	}
	else 
	{
		pTextures[index] = pTex;
	}

	SetTextureMapBuffer(pTextures[index], index);
}

void MelLib::Material::SetTextures(const std::vector<Texture*>& pTex)
{
	pTextures = pTex;

	// 増えたらリサイズ
	if (pTextures.size() != materialBuffers.size()) 
	{
		ResizeBuffer();
		CreateConstBuffer();
	}

	// バッファのMapを行う
	for (int i = 0; i < pTextures.size(); i++)
	{
		SetTextureMapBuffer(pTextures[i], i);
	}
}

void MelLib::Material::SetNormalMapTexture(Texture* pTex)
{
	
	pNormalMapTexture = pTex; 
	if (!pTex)
	{
		
		return;
	}
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
void MelLib::Material::GetPConstBuffer(std::vector<ID3D12Resource*>& buffers, const MaterialConstBufferType type) const
{
	for (int i = 0; i < materialBuffers.size(); i++)
	{
		buffers.push_back(GetPConstBuffer(type, i));
	}
}
//
//void MelLib::Material::SetTexture3D(Texture3D* pTex)
//{
//
//	pTexture3D = pTex;
//	if (!pTex)
//	{
//		return;
//	}
//	
//
//	CreateBuffer::GetInstance()->CreateShaderResourceView
//	(
//		CD3DX12_CPU_DESCRIPTOR_HANDLE
//		(
//			textureHeap->GetCPUDescriptorHandleForHeapStart(),
//			TEXTURE_3D_HANDLE_NUM,
//			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//		),
//		pTex->GetTextureBuffer()
//	);
//}


#pragma region ADSA

MelLib::ADSAMaterial::ADSAMaterial(ADSAMaterial& mtl)
{
	Create(mtl.drawData);
}

MelLib::ADSAMaterial& MelLib::ADSAMaterial::operator=(ADSAMaterial& mtl)
{
	Create(mtl.drawData);
	return * this;
}

void MelLib::ADSAMaterial::Create(const DrawData& drawData)
{
	CreateInitialize(sizeof(ADSAMaterialData));
	Map();

	//パイプライン作成
	ShaderDataSet set =
	{
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"LIB","main","gs_5_0" },
		{ L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" }
	};

	pipelineState = std::make_unique<PipelineState>();
	pipelineState->CreatePipeline
	(
		drawData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);

	this->drawData = drawData;
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
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/FbxGeometryShader.hlsl","main","gs_5_0" },
		{ L"../MyLibrary/PBRPixelShader.hlsl","main","ps_5_0" }
	};

	pipelineState->CreatePipeline
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

MelLib::PBRMaterial::PBRMaterial(PBRMaterial& mtl)
{
	Create(mtl.drawData);
}

MelLib::PBRMaterial MelLib::PBRMaterial::operator=(PBRMaterial& mtl)
{
	return PBRMaterial(mtl);
}

#pragma endregion