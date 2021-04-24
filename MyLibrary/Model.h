#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"


#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<dxgi.h>

#include"PipelineState.h"

//とりあえずこれのポインタをキーにする

//これにバッファ持たせると、終了時に確実にバッファ解放できない。
//プログラム終了時に

//これをObjModelなどに継承する
//これを引数にしても、OBJのボーンとか引き出せない。
//セットとかの関数をここに移植するしかない

//モデルクラスに継承するためのクラス
class Model
{
public:

	//定数バッファのタグ
	//"Texture" シェーダーリソースビュー t0
	//"LibraryConst" ライブラリ構造体 b0
	//"UserConst" ユーザー構造体 b1
	//"Material" マテリアル構造体 b2
	//COMMON_CONST_BUFFER 共通 b3
	enum HeapBufferTag
	{
		HEAP_TAG_TEXTURE_BUFFER,
		HEAP_TAG_COMMON_CONST_BUFFER,
		HEAP_TAG_LIBRARY_CONST_BUFFER,
		HEAP_TAG_USER_CONST_BUFFER,
		HEAP_TAG_MATERIAL_CONST_BUFFER,
		HEAP_TAG_OBJ_BONE_MATRIX_CONST_BUFFER
	};

private:

	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;

#pragma region 関数

#pragma region 生成

	void createDescriptorHeap
	(
		const int arrayNum
	);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">モデル生成数</param>
	/// <param name="modelFileObjectNum">モデルファイル(.objなど)に何個オブジェクトがあるか。</param>
	/// <param name="heapTop">ヒープのどこから生成するか</param>
	/// <param name="constDataSize">生成するバッファの構造体のバイト数</param>
	/// <param name="tags">生成するバッファの種類(並びはconstDataSizeと合わせること)</param>
	void createConstBuffer
	(
		const int modelNum,
		const int modelFileObjectNum,
		const int heapTop,
		const size_t userDataSize = 0
	);

	void createTextureBuffer
	(
		const std::vector<std::wstring>& texturePath,
		const int heapTop
	);


#pragma endregion

#pragma endregion

protected:
#pragma region 変数
	ComPtr<ID3D12PipelineState> pipeline;
	ID3D12PipelineState* currentSetPipeline;

	//[objの中のモデルごと]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;

	//インデックス
	std::vector<std::vector<USHORT>> indices;

	ComPtr<ID3D12DescriptorHeap>desHeap;

	//これスプライトみたいにヒープから呼び出さないようにして、
	//1つだけ生成するようにしたほうがいい?
	//バッファ1つだけ作って、ビューを複数作るようにする
	static ComPtr<ID3D12Resource>commonBuffers;
	
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

	std::string modelClassName;
#pragma endregion

#pragma region 関数


#pragma region バッファ生成

#pragma region 生成まとめ
	/// <summary>
	/// シェーダーに頂点情報を送るためのデータを生成します。
	/// (頂点バッファとインデックスバッファのこと)
	/// </summary>
	/// <param name="verticesSize">頂点データのバイト数</param>
	/// <param name="verticesNum">各</param>
	/// <param name="indicesNum"></param>
	void createModelVertexResources
	(
		const size_t verticesSize,
		const std::vector<size_t>& verticesNum,
		const std::vector<std::vector<USHORT>>& indicesNum
	);

	/// <summary>
	/// シェーダーに頂点情報以外のデータを送るためのデータを作成します。
	/// (ディスクリプタヒープとそこにビューを生成するバッファのこと)
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="modelNum"></param>
	/// <param name="modelFileObjectNum"></param>
	/// <param name="userDataSize"></param>
	void createModelHeapResources
	(
		const std::vector<std::wstring>& texturePath,
		const int modelNum,
		const int modelFileObjectNum,
		const size_t userDataSize = 0
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
		const int modelNum,
		void** vertexData
	);

	void unmapVertexBuffer(const int& modelNum);


	void mapIndexBuffer
	(
		const int modelNum,
		void** index
	);

	void unmapIndexBuffer(const int& modelNum);

	//これだと利用者がライブラリのバッファが書き換えられてしまう
	//ずっとマップしっぱなしでもいいのでは?
	//削除時にunmapすればいいのでは? 
	//マップしっぱなしにする場合、バッファごとにポインタ用意しないといけない
	
	//定数バッファの順番は固定化する。
	//create○○はまとめる

	//enum ConstBufferType
	//{
	//	CONSTBUFFER_LIBRARY,
	//	CONSTBUFFER_MATERIAL,
	//	CONSTBUFFER_USER
	//};
	///// <summary>
	///// 
	///// </summary>
	///// <param name="modelNum">何番目のモデルか</param>
	///// <param name="objectNum">モデルの何番目のオブジェクトか</param>
	///// <param name="structNum">どのバッファの仮想アドレスを取得するか</param>
	///// <param name="constData"></param>
	//void mapConstBuffer
	//(
	//	const int modelNum,
	//	const int objectNum,
	//	const ConstBufferType type,
	//	void** constData
	//);

	//const int objectNumは、まだ個別にセットできるようにするつもりないからこうしてる
	void mapUserConstBuffer
	(
		const int modelNum,
		void** constData
	);
#pragma endregion


#pragma endregion



#pragma region スムーズシェーディング
	void calcSmoothingNormal
	(
		std::vector<Vector3> normals
	);
#pragma endregion

#pragma endregion



public:
	Model();
	virtual ~Model();

#pragma region 開発者用関数


	static void createCommonBuffer();
	
	/// <summary>
	/// マップ用
	/// </summary>
	/// <param name="data"></param>
	static void mapCommonConstData(const CommonConstData& data);

	static void initialize();

#pragma endregion

#pragma region 描画

	void draw(const int modelNum);
#pragma endregion

#pragma region パイプライン

	void setPipeline(PipelineState* pipelineState);

#pragma endregion

	
};

