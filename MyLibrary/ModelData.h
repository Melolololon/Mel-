#pragma once

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"


//モデルの頂点、インデックス、テクスチャをまとめたクラス
class ModelData
{
private:
	static ID3D12Device* device;

	VertexBufferSet vertexBufferSet;
	int modelFileObjectNum = 0;

	//インデックス
	IndexBufferSet indexBufferSet;
	std::vector<USHORT> indices;

	//テクスチャバッファ用ヒープ
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;
	
	//読み取ったマテリアル
	Material material;

protected:

	std::vector<Texture>textures;

	/// <summary>
	/// 頂点バッファの生成を行います。
	/// </summary>
	/// <param name="verticesSize"></param>
	/// <param name="verticesNum"></param>
	void CreateVertexBuffer
	(
		const size_t verticesSize,
		const std::vector<size_t>& verticesNum
	);

	/// <summary>
	/// インデックスバッファの生成、Mapを行います。
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBuffer
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	void CteateTextureBuffer();
	void CteateTextureBufferSetColor(const Color& color);

public:
	
	ModelData();
	~ModelData();



};

