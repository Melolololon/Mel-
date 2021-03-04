#include "CreateBuffer.h"


CreateBuffer::CreateBuffer(ID3D12Device* dev, int windowWidth, int windowHeight)
{
	this->dev = dev;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}
CreateBuffer::~CreateBuffer() {}

#pragma region 頂点

void CreateBuffer::createVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<Vertex> vertices, VertexBufferSet& set)
{
	dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);



	set.vertexBuffer->Map(0, nullptr, (void**)&set.vertexMap);

	for (int j = 0; j < (int)vertices.size(); j++)
	{
		set.vertexMap[j] = vertices[j];
	}

	set.vertexBuffer->Unmap(0, nullptr);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = sizeof(Vertex) * vertices.size();
	set.vertexBufferView.StrideInBytes = sizeof(Vertex);


}


bool CreateBuffer::createPMDVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<PMDVertex> vertices, PMDVertexBufferSet& set)
{
	auto result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);

	if (result != S_OK)return false;

	set.vertexBuffer->Map(0, nullptr, (void**)&set.vertexMap);
	for (int j = 0; j < (int)vertices.size(); j++)
	{
		set.vertexMap[j] = vertices[j];
	}
	set.vertexBuffer->Unmap(0, nullptr);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = sizeof(PMDVertex) * vertices.size();
	set.vertexBufferView.StrideInBytes = sizeof(PMDVertex);

	return true;
}


bool CreateBuffer::createUserVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, VertexBufferSet& set)
{

	auto result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.vertexBuffer)
	);

	if (result != S_OK)return false;

	result = set.vertexBuffer->Map(0, nullptr, vertexData);


	//set.vertexBuffer->Unmap(0, nullptr);

	set.vertexBufferView.BufferLocation = set.vertexBuffer->GetGPUVirtualAddress();
	set.vertexBufferView.SizeInBytes = vertexSumDataSize;
	set.vertexBufferView.StrideInBytes = vertexDataSize;

	return true;
}

#pragma endregion


void CreateBuffer::createIndexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<unsigned short> indices, IndexBufferSet& set)
{
	dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.indexBuffer)
	);

	set.indexBuffer->Map(0, nullptr, (void**)&set.indexMap);
	for (int j = 0; j < (int)indices.size(); j++)
	{
		set.indexMap[j] = indices[j];
	}
	set.indexBuffer->Unmap(0, nullptr);

	set.indexBufferView.BufferLocation = set.indexBuffer->GetGPUVirtualAddress();
	set.indexBufferView.Format = DXGI_FORMAT_R16_UINT;
	set.indexBufferView.SizeInBytes = sizeof(unsigned short) * indices.size();
}


void CreateBuffer::createConstBufferSet(D3D12_HEAP_PROPERTIES cbheapprop, D3D12_RESOURCE_DESC cbresdesc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, void** constData, ConstBufferSet& set, int num)
{

	auto result = dev->CreateCommittedResource(
		&cbheapprop,
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.constBuffer[num]));

	result = set.constBuffer[num]->Map(0, nullptr, constData);

	//これを2Dか3Dかで入れるものを変える
	//set.constMaps->mat = constBufferData.mat;

	/*set.constMaps->mat.r[0].m128_f32[0] = 2.0f / windowWidth;
	set.constMaps->mat.r[1].m128_f32[1] = -2.0f / windowHeight;
	set.constMaps->mat.r[3].m128_f32[0] = -1.0f;
	set.constMaps->mat.r[3].m128_f32[1] = 1.0f;*/

	//set.constBuffer->Unmap(0, nullptr);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

	cbvDesc.BufferLocation = set.constBuffer[num]->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)set.constBuffer[num]->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc, heapHandle);
	
}

void CreateBuffer::createTextureBufferSet(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DirectX::TexMetadata metadata, const DirectX::Image* image, TextureBufferSet& set, int num)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = metadata.format;
	texResDesc.Width = metadata.width;
	texResDesc.Height = (UINT)metadata.height;
	texResDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	dev->CreateCommittedResource(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.textureBuff[num]));

	auto result = set.textureBuff[num]->WriteToSubresource
	(
		0,
		nullptr,
		image->pixels,
		(UINT)image->rowPitch,
		(UINT)image->slicePitch
	);

	//ここがおかしい?
	//Formatがおかしかった
	//またここおかしくなった
	//ハンドルがダメ?ポインタがダメ?
	//ハンドルおかしくないマジわけわからん
	//TextureBufferSetだと読み込めないが、std;;vector<TextureBufferSet>だと読み込める
	//なぜかベクターだとエラーでない。ちなみに配列は無理だった。
	//D3D12_RESOURCE_STATE_GENERIC_READを書かないといけないのにD3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCEを書いていた
	//Desc書く部分にViewを書いてたからエラー
	//Viewだと思ってたのはDescだった
	//Format違いと構造体のDescを使っていたのが問題だった
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView
	(
		set.textureBuff[num].Get(),
		&srvDesc,
		heapHandle
	);

}

void CreateBuffer::createTextureBufferSet2(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, Color color, TextureBufferSet& set, int num)
{
	D3D12_HEAP_PROPERTIES texHeapprop{};
	D3D12_RESOURCE_DESC texResDesc{};

	texHeapprop.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	texHeapprop.CreationNodeMask = 0;
	texHeapprop.VisibleNodeMask = 0;

	texResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texResDesc.Width = 20;
	texResDesc.Height = 20;
	texResDesc.DepthOrArraySize = 1;
	texResDesc.SampleDesc.Count = 1;
	texResDesc.SampleDesc.Quality = 1;
	texResDesc.MipLevels = 1;
	texResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	auto result = dev->CreateCommittedResource(
		&texHeapprop,
		D3D12_HEAP_FLAG_NONE,
		&texResDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&set.textureBuff[num]));



	std::vector<Color>c(20 * 20);
	for (auto& rgba : c)
	{
		rgba.r = color.r;
		rgba.g = color.g;
		rgba.b = color.b;
		rgba.a = color.a;

	}

	result = set.textureBuff[num]->WriteToSubresource
	(
		0,
		nullptr,
		c.data(),
		sizeof(Color) * 20,
		sizeof(Color) * c.size()
	);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView
	(
		set.textureBuff[num].Get(),
		&srvDesc,
		heapHandle
	);
}


void CreateBuffer::createDepthBufferSet(D3D12_HEAP_PROPERTIES depthheapprop, D3D12_RESOURCE_DESC depthresdesc, D3D12_CLEAR_VALUE depthclearvalue, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DepthBufferSet& set)
{


	dev->CreateCommittedResource
	(
		&depthheapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthresdesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthclearvalue,
		IID_PPV_ARGS(&set.depthBuffer)
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView
	(
		set.depthBuffer.Get(),
		&dsvDesc,
		heapHandle
	);
}
