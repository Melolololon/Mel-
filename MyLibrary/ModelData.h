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

	//インデックス
	std::vector <IndexBufferSet> indexBufferSet;

	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;

	//読み取ったマテリアル
	Material material;

	//テクスチャを使わない場合の色
	Color color;


#pragma region 頂点

	/// <summary>
	/// 頂点バッファ、ビューの生成を行います。
	/// </summary>
	/// <param name="vertexSize"></param>
	/// <param name="vertices"></param>
	void CreateVertexBufferSet
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum
	);

	void MapVertices();

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



	//モデルファイルに何個モデルがあるか
	int modelFileObjectNum = 0;

	std::vector<std::vector<USHORT>> indices;

	std::vector<std::unique_ptr<Texture>>pTextures;

	/// <summary>
	/// 頂点インデックスバッファセットの生成とMapを行います。
	/// </summary>
	/// <typeparam name="VERTEX">頂点</typeparam>
	/// <param name="vertexSize"></param>
	/// <param name="vertexNum"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	void BufferPreparationSetTexture
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<USHORT>>& indices
	);

	void BufferPreparationSetColor
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<USHORT>>& indices
	);


public:

	ModelData() {}
	~ModelData() {}

#pragma region コマンドセット用関数

	/// <summary>
	/// インデックスを取得。
	/// </summary>
	/// <returns></returns>
	const std::vector<std::vector<USHORT>>& GetIndices()const { return indices; }
	
	/// <summary>
	/// 頂点バッファを取得。
	/// </summary>
	/// <returns></returns>
	const std::vector<VertexBufferSet>& GetVertexBufferSet()const { return vertexBufferSet; }
	
	/// <summary>
	/// インデックスバッファの取得。
	/// </summary>
	/// <returns></returns>
	const std::vector<IndexBufferSet>& GetIndexBufferSet()const { return indexBufferSet; }
	
	/// <summary>
	/// ディスクリプタヒープの取得。
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

#pragma endregion

#pragma region セット

#pragma endregion


#pragma region ゲット

	/// <summary>
	/// モデルファイルに含まれているオブジェクト(モデル)の数を取得します。
	/// </summary>
	/// <returns></returns>
	int GetModelFileObjectNumber()const { return modelFileObjectNum; }

	/// <summary>
	/// マテリアルを取得します。
	/// </summary>
	/// <returns></returns>
	Material GetMaterial()const { return material; }


#pragma endregion

};

