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

//objとfbxまとめる?
//分けたほうがいい　プリミティブとかあるし
//生成と呼び出しだけ同じクラスの関数使う?

//モデルの頂点、インデックス、テクスチャをまとめたクラス
class ModelData
{
private:
	static ID3D12Device* device;

	//頂点
	std::vector<VertexBufferSet> vertexBufferSet;
	int modelFileObjectNum = 0;

	//インデックス
	std::vector < IndexBufferSet> indexBufferSet;
	std::vector<std::vector<USHORT>> indices;

	//テクスチャバッファ用ヒープ
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;
	std::vector<Texture*>pTextures;

	//読み取ったマテリアル
	Material material;

	void CreateDescriptorHeap(const UINT textureNum);

protected:

	std::vector<Texture>textures;

	/// <summary>
	/// 頂点バッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="verticesSize"></param>
	/// <param name="verticesNum"></param>
	void CreateVertexBuffer
	(
		const size_t verticesSize,
		const std::vector<size_t>& verticesNum
	);

	/// <summary>
	/// インデックスバッファ、ビューの生成、Mapを行います。
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBuffer
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	/// <summary>
	/// テクスチャバッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="pTextures"></param>
	void CteateTextureBuffer(const std::vector<Texture*>& pTextures);

	/// <summary>
	/// テクスチャバッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="color"></param>
	void CteateTextureBufferSetColor(const Color& color);

public:
	
	ModelData(){}
	~ModelData(){}

#pragma region コマンドセット用関数
	const std::vector<std::vector<USHORT>>& GetIndices()const { return indices; }

	const std::vector<VertexBufferSet>& GetVertexBufferSet()const { return vertexBufferSet; }
	const std::vector<IndexBufferSet>& GetIndexBufferSet()const { return indexBufferSet; }
	ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

#pragma endregion

};

