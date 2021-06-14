#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>


#include"PipelineState.h"
#include"Texture.h"
#include"RenderTarget.h"


struct BufferData
{
	enum BufferType
	{
		BUFFER_TYPE_NONE,//生成しない
		BUFFER_TYPE_COMMON,//全モデル共通
		BUFFER_TYPE_EACH_MODEL,//各モデルごと
		BUFFER_TYPE_EACH_MODEL_OBJECT,//各モデル各オブジェクトごと
	};

	BufferType bufferType;//バッファの数
	size_t bufferDataSize;//バッファ1つのサイズ
};

//モデルクラスに継承するためのクラス
class Model
{
public:

	//ヒープ内を可視化するためのタグ
	enum HeapBufferTag
	{
		HEAP_TAG_TEXTURE_BUFFER,//テクスチャ
		HEAP_TAG_COMMON_CONST_BUFFER,//使わん
		HEAP_TAG_LIBRARY_CONST_BUFFER,//メインのバッファ
		HEAP_TAG_MATERIAL_CONST_BUFFER,//マテリアル
		HEAP_TAG_MODEL_CONST_BUFFER,//各モデルクラスごとに用意するバッファ
		HEAP_TAG_USER_CONST_BUFFER,//利用者が用意したデータのバッファ
	};

	

private:
#pragma region 変数
	//全共通
	int commonConstBufferNum = 0;
	//モデルごと
	int eachModelConstBufferNum = 0;
	//モデルのオブジェクトごと
	int eachModelObjectConstBufferNum = 0;
	BufferData::BufferType modelConstBufferType;
	BufferData::BufferType userConstBufferType;

	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;

	//これスプライトみたいにヒープから呼び出さないようにして、
	//1つだけ生成するようにしたほうがいい?
	//バッファ1つだけ作って、ビューを複数作るようにする
	//static ComPtr<ID3D12Resource>commonBuffers;
	static ComPtr<ID3D12RootSignature>rootSignature;

	static DirectX::XMMATRIX viewAndProjectionMat;
	static DirectX::XMMATRIX cameraRotateMat;
	
	static CommonConstData commonConstData;
	static std::vector<Model*> pModels;

#pragma endregion

#pragma region 関数
	void MapCommonConstData();

#pragma region 生成

	void CreateDescriptorHeap
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
	void CreateConstBuffer
	(
		const int modelNum,
		const int modelFileObjectNum,
		const int heapTop,
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void CreateTextureBuffer
	(
		const std::vector<Texture*>& textures,
		const int heapTop
	);

	void Model::CreateOneColorTextureBuffer
	(
		const Color& color,
		const int heapTop
	);

#pragma endregion

#pragma endregion

protected:
#pragma region 変数
	int modelNum;//生成数
	int modelObjectNum;//ファイル内のオブジェクト数

	//テクスチャバッファ数
	int textureBufferNum;

	std::vector<ComPtr<ID3D12PipelineState>> pipeline;

	//[objの中のモデルごと]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;

	//インデックス
	std::vector<std::vector<USHORT>> indices;

	ComPtr<ID3D12DescriptorHeap>desHeap;

	
	std::vector<std::vector<ComPtr<ID3D12Resource>>> constBuffer;//メイン(基本的な情報)
	std::vector<std::vector<ComPtr<ID3D12Resource>>> materialConstBuffer;//マテリアル
	std::vector<std::vector<ComPtr<ID3D12Resource>>> userConstBuffer;//ユーザー
	std::vector<std::vector<ComPtr<ID3D12Resource>>> modelConstBuffer;//モデル特有
	


	//テクスチャ側に持たせたからコメントアウト
	//ヒープにシェーダーリソースビューを作らずに描画するのが不可能だったので、戻した
	std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	
	//ディスクリプタヒープのバッファを可視化するためのもの
	std::vector<HeapBufferTag>heapTags;


	std::vector <Model*> parentModel;
	std::vector <int> parentModelNumbers;


	//定数にセットする座標などの値
	//[モデルごと][モデル内のオブジェクト数]
	std::vector <std::vector<ModelConstData>>modelConstDatas;
	//[モデル内のオブジェクトごと]
	std::vector<Material> materials;

#pragma endregion

#pragma region 関数

	void DrawCommonProcessing(const int modelNum, const std::string& rtName);
	void MapConstData(const int modelNum, const Camera* camera);
	void SetCmdList(const int modelNum);


#pragma region バッファ生成

#pragma region 生成まとめ
	//CreateModelVertexResourcesとCreateModelHeapResourcesまとめていいかも

	/// <summary>
	/// シェーダーに頂点情報を送るためのデータを生成します。
	/// (頂点バッファとインデックスバッファのこと)
	/// </summary>
	/// <param name="verticesSize">頂点データのバイト数</param>
	/// <param name="verticesNum">各</param>
	/// <param name="indicesNum"></param>
	void CreateModelVertexResources
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
	void CreateModelHeapResourcesSetTexture
	(
		const std::vector<Texture*>& pTextures,
		const int modelNum,
		const int modelFileObjectNum,
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void CreateModelHeapResourcesSelectColor
	(
		const Color& color,
		const int modelNum,
		const int modelFileObjectNum,
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void BufferCreatePreparation
	(
		const int modelNum,
		const int modelFileObjectNum,
		const size_t textureNum,
		BufferData* modelBufferData,
		BufferData* userBufferData 
	);
#pragma endregion


	//自作頂点使うときに継承しないようにしたから、
	//マップをパブリックに移動
	//継承させるため、protectedに戻した
#pragma region Map

#pragma region マップ
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">モデル番号(obj内のどのモデルか)</param>
	/// <param name="vertexStruct">GPUの仮想アドレスを格納するポインタのポインタ</param>
	void MapVertexBuffer
	(
		const int modelNum,
		void** vertexData
	);

	void UnmapVertexBuffer(const int& modelNum);


	void MapIndexBuffer
	(
		const int modelNum,
		void** index
	);

	void UnmapIndexBuffer(const int& modelNum);

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
	void MapUserConstBuffer
	(
		const int modelNum,
		void** constData
	);
#pragma endregion

#pragma endregion

#pragma endregion



#pragma region スムーズシェーディング
	void CalcSmoothingNormal
	(
		std::vector<Vector3> normals
	);
#pragma endregion

#pragma endregion

public:
	Model();
	virtual ~Model();

#pragma region 開発者用関数

	static void SetCommonConstData(const CommonConstData& data);


	static void Initialize
	(
		ID3D12Device* dev,
		std::vector<ID3D12GraphicsCommandList*> cmdList
	);

	

	static void SetViewAndProjectionMatrix(const DirectX::XMMATRIX& mat) { viewAndProjectionMat = mat; }
	static void SetCameraRotateMat(const DirectX::XMMATRIX& mat) { cameraRotateMat = mat; }
#pragma endregion

	 

#pragma region 操作
	void SetPosition(const Vector3& position, const int modelNum);
	void SetAngle(const Vector3& angle,const int modelNum);
	void SetScale(const Vector3& scale, const int modelNum);
#pragma endregion

#pragma region 描画

	virtual void Draw(const int modelNum,const std::string& rtName = "");
	virtual void AllDraw();
#pragma endregion

#pragma region パイプライン

	void SetPipeline(PipelineState* pipelineState, const int modelNum);
	void SetPipelineAllSet(PipelineState* pipelineState);

#pragma endregion

	
};

