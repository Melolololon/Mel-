#pragma once
#include<Windows.h>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<random>
#include<ctime>
#include<string>
#include<unordered_map>
#include<wrl.h>


#include"DirectInput.h"
#include"CreateBuffer.h"
#include"CreatePolygon.h"
#include"CreatePipeline.h"
#include"ModelLoader.h"
#include"CreateCamera.h"
#include"Vector.h"
#include"DirectXStruct.h"
#include"DirectXTexLoader.h"
#include"DrawGraphicStruct.h"

#define DIRECTINPUT_VERSION 0x0800
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<dxgidebug.h>
#include<dinput.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")


//これDirectXStructに移して、モデルの頂点バッファ作るところでこのenumセットしてもいいかも
//これパイプラインセットするときに設定するようにする?
//その場合、ヒープの構造体も作ったほうがいい?
enum VertexType
{
	VERTEX_TYPE_NORMAL,//座標、uv、法線のみ
	VERTEX_TYPE_OBJ_ANIMATION,//座標、uv、法線、ボーン番号
	VERTEX_TYPE_USER_VERTEX,//利用者の自作データ

};

using namespace Microsoft::WRL;
class DirectX12
{

	enum DrawType
	{
		none,
		box,
		circle,
	};

	enum Dimension
	{
		dimention2D,
		dimention3D
	};

private:
	float clearColor[4];

#pragma region 現在の描画コマンド状態
	enum RSState
	{
		RS_MODEL,
		RS_SPRITE,
		RS_POINT,
	};
#pragma endregion

	DirectX::XMMATRIX cameraMat;

#pragma region Windows
	HWND hwnd;
	int winWidth;
	int winHeight;
#pragma endregion

#pragma region 基本的なオブジェクト

	//DirectX変数
	HRESULT result;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;


#pragma region RTV

	//メイン
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Resource> backBuffer[2];
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;

#pragma endregion

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	D3D12_RESOURCE_BARRIER barrierDesc;


	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12RootSignature> spriteRootsignature;
	ComPtr<ID3D12RootSignature> pointRootsignature;

	//ユーザー作成のパイプライン
	//上の単体のやつもこっちにまとめる?
	//vectorじゃなくて普通の配列にする?
	std::vector<ComPtr<ID3D12PipelineState>> pipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> spritePipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> pointPipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> objPipelineStates;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC spriteGpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pointGpipeline;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;
	int startPipelineCreateNum;//Initialize時に作る3Dのパイプライン数

	int pipelineNum;
	int spritePipelineNum;


#pragma endregion

#pragma region バッファ

#pragma region 頂点 インデックス
	//[モデルごと][objの中のモデルごと]
	std::unordered_map<std::string,std::vector<VertexBufferSet>> vertexBufferSet;
	std::unordered_map<std::string,std::vector<IndexBufferSet>>indexBufferSet;


#pragma endregion

#pragma region ヒープに入れるバッファ

	std::vector<std::vector<ConstBufferSet>> constBufferSet;
	std::vector<ComPtr<ID3D12Resource>>commonBuffer;
	std::vector<TextureBufferSet> textureBufferSet;

	std::vector<VertexBufferSet> spriteVertexBuffSet;
	std::vector<IndexBufferSet> spriteIndexBufferSet;//これ固定だから複数いらない?
	std::vector<ConstBufferSet> spriteConstBufferSet;
	std::vector<TextureBufferSet> spriteTextureBufferSet;

	std::vector< std::vector<VertexBufferSet>> pointVertexBuffSet;
	std::vector<ConstBufferSet> pointConstBufferSet;
	std::vector<TextureBufferSet> pointTextureBufferSet;

	std::vector<TextureBufferSet>spriteFontTextureBufferSet;

#pragma endregion

	DepthBufferSet depthBufferSet;
#pragma endregion

#pragma region ヒープ

	std::vector<ComPtr<ID3D12DescriptorHeap>> basicHeaps;
	ComPtr<ID3D12DescriptorHeap> spriteHeap;
	ComPtr<ID3D12DescriptorHeap> pointHeap;
	ComPtr<ID3D12DescriptorHeap> spriteFontHeap;
	ComPtr<ID3D12DescriptorHeap> depthHeap;


#pragma endregion

	//自作パイプライン作成時に使用するdesc
	D3D12_INPUT_ELEMENT_DESC* userDesc;

#pragma region バッファ作成などのクラス

	CreateBuffer* createBuffer;
	CreatePipeline* createPipeline;
	CreatePolygon* createPolygon;
	CreateCamera* mainCamera;

#pragma endregion


#pragma region 頂点

#pragma region 3D

	//std::vector<std::vector<std::vector<Vertex>>> vertices;//[モデルごと][objにあるモデルごと][頂点ごと]
	//std::vector<std::vector<std::vector<OBJAnimationVertex>>> objAniVertices;//Vertexに加え、ボーン番号を追加
	//std::vector<std::vector<std::vector<USHORT>>> indices;


	std::unordered_map<std::string, std::vector<std::vector<Vertex>>> vertices;//[モデルごと(keyでアクセス)][objにあるモデルごと][頂点ごと]
	std::unordered_map<std::string, std::vector<std::vector<OBJAnimationVertex>>> objAniVertices;//Vertexに加え、ボーン番号を追加

	std::unordered_map<std::string, std::vector<std::vector<USHORT>>> indices;
#pragma endregion

	std::vector<std::vector<Vertex>> spriteVertices;
	std::vector<std::vector<unsigned short>> spriteIndices;
	std::vector<std::vector<PointVertex>>pointVertices;//[点の番号ごと][点ごと]
	PointVertex* pointVertexMapData;

	//スムースシェーディング用データ
	std::unordered_map<std::string, std::vector<std::vector<DirectX::XMFLOAT3>>> smoothNormal;//法線を平均した頂点データ
	
	//[obj内のオブジェクト分]
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//スムースシェーディング用関数
	/// <summary>
	/// スムースシェーディングを行うための計算をします
	/// </summary>
	void calcSmoothingNormals(const std::string key);


#pragma endregion


#pragma region ヒープ関係

#pragma region 定数バッファ関係

	//定数バッファデータ
	ConstBufferData* constData3D;
	//共通
	CommonConstData* commonConstData3D;
	//ユーザー
	void** userConstData3D;
	unsigned int userConstDataSize3D;
	//マテリアル用
	MaterialConstBuffData* materialData;

	//下なくす
	std::vector<ConstBufferData*>spriteConstBufferDatas;

	//点用の構造体
	PointConstBufferData* constDataPoint;

	//定数バッファのタグ(ルートパラメーターセット用)
	//"Texture" シェーダーリソースビュー t0
	//"LibraryConst" ライブラリ構造体 b0
	//"UserConst" ユーザー構造体 b1
	//"Material" マテリアル構造体 b2
	enum ConstBufferTag
	{
		TEXTURE_BUFFER,
		COMMON_BUFFER,
		LIBRARY_CONST_BUFFER,
		USER_CONST_BUFFER,
		MATERIAL_CONST_BUFFER
	};
	std::vector<std::vector<ConstBufferTag>>heapTags;

#pragma endregion

#pragma region テクスチャ関係
	//画像データ
	std::vector<DirectX::TexMetadata> textureData;
	std::vector<DirectX::TexMetadata> spriteTextureData;

#pragma endregion


#pragma endregion



	//送られてきた情報
	std::vector<PolyData> polyDatas;
	std::vector<HeapData> despDatas;
	//取得したスプライトのポインタ
	std::vector<int*>spriteP;
	std::vector<int*>pointP;

	//これいらない
	std::vector<DirectX::XMFLOAT2>spriteSizes;

#pragma region カウント変数

#pragma region 頂点


	//struct Vertexでバッファを作った回数
	int createNormalVertexBufferCount;

	//struct OBJAnimationVertexでバッファを作った回数
	int createOBJAnimationVertexBufferCounter;

#pragma endregion

#pragma region それ以外


	int createHeapCount;
	int loadTextureCounter;
	int createSpriteCounter;
	int spriteFontDrawCounter;

#pragma endregion

#pragma endregion


#pragma region カメラ
	CameraData mainCameraData;


	DirectX::XMFLOAT3 eyeVelocity;
	DirectX::XMFLOAT3 targetVelocity;


#pragma endregion

#pragma region ライト

	DirectX::XMFLOAT3 lightVector;//ライトのベクトル
	DirectX::XMFLOAT3 lightColor;
#pragma endregion

#pragma region 見た目
	bool isPlane;//平面化するか

	bool isBillBoardX;
	bool isBillBoardY;
	bool isBillBoardZ;

	bool smoothing;
#pragma endregion


#pragma region オブジェクト情報


	std::vector<std::vector<DirectX::XMFLOAT3>>position;
	std::vector<std::vector<DirectX::XMFLOAT3>>scale;
	std::vector<std::vector<DirectX::XMFLOAT3>>angle;
	std::vector<std::vector<DirectX::XMFLOAT4>>addColor;
	std::vector<std::vector<DirectX::XMFLOAT4>>subColor;
	std::vector<std::vector<DirectX::XMFLOAT4>>mulColor;
	std::vector<std::vector<float>>exs;
	std::vector< std::vector<Material>>materials;


	//std::vector<DirectX::XMVECTOR> spritePosition;
	std::vector<DirectX::XMFLOAT2> spriteScale;
	std::vector<float> spriteAngle;



#pragma endregion

#pragma region スプライトフォント
	std::vector<SpriteFontData>spriteFontData;
#pragma endregion

#pragma region 親子構造

	//オブジェクトの親を保存する配列
	std::vector<std::vector<int>>parentHeaps;
	std::vector<std::vector<int>>parentNums;
#pragma endregion

#pragma region ポストエフェクト
	//カメラの影響を与えるかどうか
	bool postEffectCametaFlag;

	//ポストエフェクト用リソース(レンダーターゲット兼テクスチャ)
	std::vector<ComPtr<ID3D12Resource>>postEffectResources;

	//ポストエフェクト用ヒープ
	//レンダーターゲットごとに用意したほうがよさそう(自作構造体バッファのせいで調整大変だから)
	ComPtr<ID3D12DescriptorHeap>postEffectHeap;//テクスチャ + ポストエフェクト分の定数バッファビュー
	ComPtr<ID3D12DescriptorHeap>postEffectRTVHeap;

	//レンダリング先板ポリ情報
	std::vector<Vertex> postEffectVertex;
	VertexBufferSet postEffectVertexBufferSet;
	std::vector<ComPtr<ID3D12PipelineState>>postEffectPipeline;
	ComPtr<ID3D12RootSignature>postEffectRootSigneture;

	//生成数
	int currentPostEffectPipeline;

	//値格納用
	std::vector<WorldMatData>postEffectWorldMatData;

	//Map用
	PostEffectConstData* postEffectConstDataP;
	std::vector<ConstBufferSet>postEfectConstBuffers;//行列とか送る用
	
#pragma endregion

#pragma region 2枚目のポストエフェクト用レンダーターゲット(仮)
	//ポストエフェクト用リソース(レンダーターゲット兼テクスチャ)
	std::vector<ComPtr<ID3D12Resource>>toonShaderResources;

	//ポストエフェクト用ヒープ
	ComPtr<ID3D12DescriptorHeap>toonShaderHeap;
	ComPtr<ID3D12DescriptorHeap>toonShaderRTVHeap;

	//レンダリング先板ポリ情報
	//頂点とルートシグネチャは使いまわす
	std::vector<ComPtr<ID3D12PipelineState>>toonShaderPipeline;
#pragma endregion



#pragma region private関数
	void resizeObjectData(int objectNum);

	void createHeap();

	void createCommonBuffer(const int& texNum);

	/// <summary>
	/// ライブラリがパイプラインを作成します
	/// </summary>
	void create3DObjectPipeline();
#pragma endregion





public:
	DirectX12(HWND hwnd, int windouWidth, int windowHeight);
	~DirectX12();

#pragma region 初期化などの必須処理

	/// <summary>
	/// 初期化
	/// </summary>
	void initialize();

	/// <summary>
	/// 描画コマンドなどを呼び出す前に呼ぶ処理
	/// </summary>
	void preparationToDraw();

	//描画処理。コマンドリストの命令を実行したりする
	void draw();

	//番号取得
	//これ引数でどのクリエイト番号取得するか選べるようにする
	CreateNumberSet getCreateNumber();

#pragma endregion


	//画面の色受け取り
	void setScreenColor(Color screenColor);

	//スプライトのサイズ取得
	DirectX::XMFLOAT2 getTextureSize(int textureHandle);


	//これいらん
#pragma region パイプラインの設定

	void setDespTestFlag(bool flag);

#pragma endregion

#pragma region ユーザーパイプライン関係

	int getStartPipelineCreateNum();

	/// <summary>
	/// 自作シェーダーを使ってパイプラインを作ります
	/// </summary>
	/// <param name="vShaderData"></param>
	/// <param name="gSyaderData"></param>
	/// <param name="pShaderData"></param>
	bool createUserPipelineState(
		PipelineData pipelineData,
		ShaderData vShaderData,
		ShaderData gSyaderData,
		ShaderData pShaderData,
		bool useUserInputLayout);

	/// <summary>
	/// パイプラインを切り替えます
	/// </summary>
	/// <param name="num"></param>
	void setPipelineNumber(int num);

	//ライブラリ制の定数データを使う場合、const char*の文字列を呼び出して渡す
	void setConstMapData(void** dataP, unsigned int dataSize);

	//ワールド、ビュー、プロジェクション行列を乗算したものを受け取ります
	void getMatrix(float matrix[4][4], int heapNum, int number);

	//ワールドは乗算しない
	void getCameraMatrix(float matrix[4][4]);

	/// <summary>
/// インプットレイアウトの設定をします
/// </summary>
/// <param name="semantics">セマンティクス名</param>
/// <param name="num">データ数(Vector3の場合、三次元なので3を入力)</param>
	void setInputLayout(const char* semantics, int num);

	/// <summary>
/// 現在のインプットレイアウトを削除します
/// </summary>
	void deleteInputLayout();


#pragma region ポストエフェクト
	void setPostEffectPipeline(const int& num);

	bool createUserPostEffectPipelineState(const ShaderData& pShaderData);
#pragma endregion


#pragma endregion

#pragma region バッファ作成
	void createPoint(int createNum, int* point);
	
	//typeに応じてどのcountを++するか決める
	void createPolygonData(PolyData polygonData,const std::string& key);
	void createHeapData(HeapData despData, bool setConstDataFlag);

#pragma region ユーザー
	/// <summary>
	/// ライブラリの構造体を使用し、自分で頂点情報を設定してオブジェクトを生成します
	/// </summary>
	/// <param name="vertex"></param>
	/// <param name="p"></param>
	void addUserVertex(std::vector<Vector3>& vertexPos, std::vector<Vector2>& vertexUV, const std::string& key);

	void addUserIndex(std::vector<unsigned short>& index, const std::string& key);

	/// <summary>
	/// 
	/// 自作のデータを使用し、自分で頂点情報を設定してオブジェクトを生成します
	/// </summary>
	/// <param name="vertexData"></param>
	/// <param name="vertexDataSize"></param>
	/// <param name="index"></param>
	/// <param name="polyData"></param>
	void createUserPolygon
	(
		void** vertexData,
		unsigned int vertexDataSize,
		unsigned int vertexSumDataSize,
		std::vector<unsigned short>&index, 
		PolyData polyData, 
		const std::string& key
	);
#pragma endregion

#pragma region モデル
	VertexType loadOBJVertex
	(
		const char* path, 
		bool loadUV,
		bool loadNormal,
		std::string* materialFireName,
		PolyData data, 
		const std::string& key
	);

	void loadOBJMaterial(std::string materialDirectoryPath, std::string materialFileName, HeapData heapData, bool setConstDataFlag);

	//これいらない
	void loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, PolyData vertData, HeapData heapData);
#pragma endregion


	void loadSpriteFont(const wchar_t* texturePath, DirectX::XMFLOAT2 lineNum, DirectX::XMFLOAT2 fontSize);

	void loadTexture(const wchar_t* texturePath, Color color);

	//fontは、createするものがスプライトフォントかどうか
	void createSprite(int* spr, bool font);


#pragma endregion

#pragma region 描画
	//描画時じゃなくて座標とかセットしたときに毎回マップする?
//そうすると処理が遅くなる?
	void setCmdList(int polyNum, int despNum, int number);
	void map(int polyNum, int despNumber, int number);

	void spriteSetCmdList(int spriteNum, int textureNum);

	void spriteMap(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, int spriteNum, int textureNum);

	void pointSetCmdList(DirectX::XMFLOAT3 pos, int pointNum, int textureNum, int num);

#pragma endregion

#pragma region 削除
	void deletePolygonData(int polyNum);
	void deleteHeapData(int despNum);

	void deleteSprite(int sprite);
#pragma endregion

#pragma region カメラ
	void setCameraDataMatrixPoint(Vector3 eye, Vector3 target, Vector3 up);
	void setCameraData(Vector3 eye, Vector3 target, Vector3 up);

	void setCameraVelocity(DirectX::XMFLOAT3 eyeVelocity, DirectX::XMFLOAT3 targetVelocity);

	void setCameraAngre(DirectX::XMFLOAT3 eyeAngle, DirectX::XMFLOAT3 targetAngle, DirectX::XMFLOAT3 upAngle);


	void setNearAndFar(float nearNum, float farNum);
#pragma endregion

#pragma region 見た目操作

#pragma region スムースシェーディング
	void setSmoothingFlag(bool flag);
#pragma endregion


	void setMulColor(Color color, int despNum, int number);
	void setAddColor(Color color, int despNum, int number);
	void setSubColor(Color color, int despNum, int number);

	void setIsPlane(bool flag);
	void setIsBillboard(bool x, bool y, bool z);

	void setSpriteMulColor(Color color, int spriteNum);
	void setSpriteAddColor(Color color, int spriteNum);
	void setSpriteSubColor(Color color, int spriteNum);



	void setPointMulColor(Color color, int pointNum, int num);



#pragma endregion

#pragma region ライト操作
	void setLightVector(DirectX::XMFLOAT3 vector);

	void setLightColor(Color lightColor);
#pragma endregion

#pragma region 操作
	void setObjectPosition(DirectX::XMFLOAT3 position, int despNum, int number);
	void setObjectScale(DirectX::XMFLOAT3 angle, int despNum, int number);
	void setObjectAngle(DirectX::XMFLOAT3 angle, int despNum, int number);
	void setObjectEX(float objectEX, int despNum, int number);

	void spriteSetObjectPosition(DirectX::XMFLOAT2 position, int spriteNum);
	void spriteSetObjectScale(DirectX::XMFLOAT2 scale, int spriteNum);
	void spriteSetObjectAngle(float angle, int spriteNum);

	void changeSpriteSize(DirectX::XMFLOAT2 size, int spriteData);


	void setPointScale(DirectX::XMFLOAT2 scale, int pointNum, int num);



#pragma region ポストエフェクト

	/// <summary>
	/// レンダーターゲットの座標を変更します
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rtNum">どのレンダーターゲットを指定するか(今は意味なし)</param>
	 void setRenderTargerPosition(const DirectX::XMFLOAT3& pos, const int& rtNum);

	 void setRenderTargetAngle(const DirectX::XMFLOAT3& angle, const int& rtNum);

	 void setRenderTargetScale(const DirectX::XMFLOAT3& scale, const int& rtNum);

	 void setPostEffectCameraFlag(const bool& flag, const int& rtNum);
#pragma endregion

#pragma endregion

#pragma region 頂座標取得
	std::vector<std::vector<DirectX::XMFLOAT3>> getObjectVertexPosition(int vertData);

	bool overrideWriteVertexPosition(std::vector<std::vector<DirectX::XMFLOAT3>> vertPos, int vertNum);
#pragma endregion


#pragma region アニメーション

	void setAnimation(int polyNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);
	void setAnimation2(int polyNum, int dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY);

	void setSpriteAnimationVertex(int spriteNum, int textureNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);
	void setSpriteAnimationVertex2(int spriteNum, int textureNum, int posX, int posY, int areaWidth, int areaHeight, int startAreaX, int startAreaY, int endAreaX, int endAreaY);
#pragma endregion



#pragma region 行列による変換
	/// <summary>
	/// ベクトルの向きを回転行列でカメラのアングル分回転して変更し、戻します
	/// </summary>
	/// <param name="vector">ベクトル</param>
	/// <param name="flag">正規化するかどうか(trueで正規化する)</param>
	/// <returns></returns>
	DirectX::XMFLOAT3 matchEyeVelocityToCamera(DirectX::XMFLOAT3 eyeVector, bool flag);

	/// <summary>
	///	行列で回転、移動させたカメラの座標を戻します
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 getRotateCameraPosition();


	/// <summary>
	/// 行列で回転、移動させたカメラの注視点を戻します
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 getRotateCameraTarget();
#pragma endregion

#pragma region 文字表示
	void drawSpriteFontString(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, std::string text, int spriteFontTextureNum);
#pragma endregion

#pragma region 親子構造
	//ルートオブジェクトの子の子にルートオブジェクトと子の行列を乗算するために、
	//親のオブジェクトを記録する配列作る?

	void setParent(int heapNum, int number, int parentObjHeapNum, int parentNum);

#pragma endregion

#pragma region 計算

	void calculationNormal
	(
		DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT3 pos3,
		DirectX::XMFLOAT3& normal1, DirectX::XMFLOAT3& normal2, DirectX::XMFLOAT3& normal3
	);
#pragma endregion


};

