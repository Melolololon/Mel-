#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include<DirectXMath.h>
#include<vector>
#include"DirectXStruct.h"


struct ModelVerticesData
{
	//[objにあるモデルごと][頂点ごと]
	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<DirectX::XMFLOAT3>>smoothNormal;
	std::vector<std::vector<USHORT>> indices;

	//[objの中のモデルごと]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;
};



struct ModelHeapData
{
	std::vector < ModelConstData>modelConstData;
	MaterialConstBuffData material;
	DirectX::TexMetadata textureData;

	std::vector<ConstBufferSet> constBufferSet;
	ComPtr<ID3D12Resource>commonBuffers;
	TextureBufferSet textureBufferSet;


	ComPtr<ID3D12DescriptorHeap> basicHeaps;

	//定数バッファのタグ(ルートパラメーターセット用)
//"Texture" シェーダーリソースビュー t0
//"LibraryConst" ライブラリ構造体 b0
//"UserConst" ユーザー構造体 b1
//"Material" マテリアル構造体 b2
//COMMON_CONST_BUFFER 共通 b3
	enum ConstBufferTag
	{
		TEXTURE_BUFFER,
		COMMON_CONST_BUFFER,
		LIBRARY_CONST_BUFFER,
		USER_CONST_BUFFER,
		MATERIAL_CONST_BUFFER,
		OBJ_BONE_MATRIX_CONST_BUFFER
	};
	std::vector<ConstBufferTag>heapTags;

};





//これにバッファ持たせると、終了時に確実にバッファ解放できない。
//プログラム終了時に

//これをObjModelなどに継承する
//これを引数にしても、OBJのボーンとか引き出せない。
//セットとかの関数をここに移植するしかない
//それか、引数分描画関数を用意(オーバーロードで)
class Model3D
{
private:
	ModelVerticesData modelVerticesData;
	ModelHeapData modelHeapData;
	

	std::vector <Model3D*> parentModel;
	std::vector <int> parentModelNumbers;




public:
	/*Model3D();
	~Model3D();*/

	void setModelVerticesData(const ModelVerticesData& data)
	{
		modelVerticesData = data;
	}
	void setModelHeapData(const ModelHeapData& data)
	{
		modelHeapData = data;
	}


};

