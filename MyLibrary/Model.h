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
	enum ConstBufferTag
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

	//これスプライトみたいにヒープから呼び出さないようにして、
	//1つだけ生成するようにしたほうがいい?
	ComPtr<ID3D12Resource>commonBuffers;

	//[ヒープ番号]
	std::vector<ConstBufferSet> constBufferSet;
	TextureBufferSet textureBufferSet;
	//ディスクリプタヒープのバッファを可視化するためのもの
	std::vector<ConstBufferTag>heapTags;


	std::vector <Model*> parentModel;
	std::vector <int> parentModelNumbers;


	//定数にセットする座標などの値
	std::vector <ModelConstData>modelConstData;

	std::vector<Material> materials;
public:
	Model();
	virtual ~Model();

	
};

