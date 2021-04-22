#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"


#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<dxgi.h>

//とりあえずこれのポインタをキーにする

//これにバッファ持たせると、終了時に確実にバッファ解放できない。
//プログラム終了時に

//これをObjModelなどに継承する
//これを引数にしても、OBJのボーンとか引き出せない。
//セットとかの関数をここに移植するしかない
class Model
{
public:

	//定数バッファのタグ(ルートパラメーターセット用)
	//"Texture" シェーダーリソースビュー t0
	//"LibraryConst" ライブラリ構造体 b0
	//"UserConst" ユーザー構造体 b1
	//"Material" マテリアル構造体 b2
	//COMMON_CONST_BUFFER 共通 b3
	enum HeapBufferTag
	{
		TEXTURE_BUFFER,
		COMMON_CONST_BUFFER,
		LIBRARY_CONST_BUFFER,
		USER_CONST_BUFFER,
		MATERIAL_CONST_BUFFER,
		OBJ_BONE_MATRIX_CONST_BUFFER
	};

protected:
	static ID3D12Device* device;
	static std::vector<ID3D12CommandList*>cmdLists;


	//[objの中のモデルごと]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;

	ComPtr<ID3D12DescriptorHeap>desHeap;

	//これスプライトみたいにヒープから呼び出さないようにして、
	//1つだけ生成するようにしたほうがいい?
	ComPtr<ID3D12Resource>commonBuffers;
	
	//[ヒープの番号(heapNum)ごと][obj内のモデルごと][バッファごと]
	std::vector<std::vector<std::vector<ComPtr<ID3D12Resource>>>> constBuffer;
	std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	//ディスクリプタヒープのバッファを可視化するためのもの
	std::vector<HeapBufferTag>heapTags;


	std::vector <Model*> parentModel;
	std::vector <int> parentModelNumbers;


	//定数にセットする座標などの値
	std::vector <ModelConstData>modelConstData;

	std::vector<Material> materials;


#pragma region バッファ

#pragma region 生成

	//生成を頂点関係、ヒープ関係で完全にまとめ、
	//Buffer作成関数はprivateにする

	/// <summary>
	/// 頂点バッファの生成
	/// </summary>
	/// <param name="verticesSize"></param>
	/// <param name="verticesNum"></param>
	void createVertexBuffer
	(
		const size_t& verticesSize,
		const size_t& verticesNum
	);

	/// <summary>
	/// インデックスバッファ作成
	/// </summary>
	/// <param name="indicesNum"></param>
	void createIndexBuffer(const std::vector<USHORT>& indicesNum);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">生成数</param>
	/// <param name="modelFileObjectNum">モデルファイルに何個オブジェクトがあるか。</param>
	/// <param name="heapTop"></param>
	/// <param name="constDataSize"></param>
	/// <param name="constData"></param>
	void createConstBuffer
	(
		const int& modelNum,
		const int& modelFileObjectNum,
		const int& heapTop,
		const std::vector<size_t>& constDataSize,
		std::vector<void**> constData
	);

	void createTextureBuffer
	(
		std::wstring path
	);


#pragma endregion

#pragma region マップ
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">モデル番号(obj内のどのモデルか)</param>
	/// <param name="vertexStruct">GPUの仮想アドレスを格納するポインタのポインタ</param>
	void mapVertexBuffer
	(
		const int& modelNum,
		void** vertexStruct
	);

	void unmapVertexBuffer(const int& modelNum);


	void mapIndexBuffer
	(
		const int& modelNum,
		void** index
	);

	void unmapIndexBuffer(const int& modelNum);
#pragma endregion


#pragma endregion

#pragma region ヒープ

#pragma region 生成
	
	void createDescriptorHeap
	(
		const int& arrayNum
	);

#pragma endregion


#pragma endregion


#pragma region スムーズシェーディング
	void calcSmoothingNormal
	(
		std::vector<Vector3> normals
	);
#pragma endregion

public:
	Model();
	virtual ~Model();

	
};

