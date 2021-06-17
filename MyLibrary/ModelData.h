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

	//テクスチャバッファ用ヒープ
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;

	//読み取ったマテリアル
	Material material;

	//テクスチャを使わない場合の色
	Color color;


#pragma region 頂点


	template<class VERTEX>
	/// <summary>
	/// 頂点バッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="vertexSize"></param>
	/// <param name="vertices"></param>
	void CreateVertexBufferSet
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<VERTEX>>& vertices
	);

	template<class VERTEX>
	void MapVertices(const std::vector<std::vector<VERTEX>>& vertices);

#pragma endregion

#pragma region インデックス


	/// <summary>
	/// インデックスバッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBufferSet
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	void MapIndices(const std::vector<std::vector<USHORT>>& indices);
#pragma endregion

#pragma region テクスチャ

	/// <summary>
	/// ディスクリプタヒープの生成
	/// </summary>
	/// <param name="textureNum"></param>
	void CreateDescriptorHeap(const UINT textureNum);

	/// <summary>
	/// テクスチャバッファ、ビューの生成を行います。
	/// </summary>
	void CteateTextureBufferAndViewSetTexture();

	/// <summary>
	/// テクスチャバッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="color"></param>
	void CteateTextureBufferAndViewSetColor();

#pragma endregion



protected:

	std::vector<std::unique_ptr<Texture>>pTextures;
	std::vector<std::vector<USHORT>> indices;
	
	/// <summary>
	/// 頂点インデックスバッファセットの生成とMapを行います。
	/// </summary>
	/// <typeparam name="VERTEX">頂点</typeparam>
	/// <param name="vertexSize"></param>
	/// <param name="vertexNum"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	template<class VERTEX>
	void BufferPreparationSetTexture
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<VERTEX>>& vertices,
		const std::vector<std::vector<USHORT>>& indices
	);

	template<class VERTEX>
	void BufferPreparationSetColor
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<VERTEX>>& vertices,
		const std::vector<std::vector<USHORT>>& indices
	);

	
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

