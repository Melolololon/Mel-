#pragma once
#include"DirectXStruct.h"
#include"Texture.h"
#include"Vector.h"

#include<d3d12.h>
#include<dxgi.h>
#include<d3dx12.h>
#include<vector>
#include<array>

//頂点使いまわそうか考えたが、
//sizeとかuvとかfloat4にしないといけなくて、
//総合的に使用メモリ多そうだから毎回生成する
//使いまわさないとuvを定数バッファで指定しないといけないから、
//分けたほうがいいかも
//四角形GPUに送ってそこから分割したほうがいいかも
//スプライトだったらテッセレータで分割してバラバラにしたほうがいいかも


//これを継承して2D、3Dスプライトクラスを作る
//Drawは仮想関数。それぞれにmap処理を書く
//

class Sprite
{
private:

#pragma region 変数
	static const UINT MAX_TEXTURE_LOAD_NUM;
	
	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	static ComPtr<ID3D12RootSignature>rootSignature;
	static ComPtr<ID3D12PipelineState>defaultPipeline;


#pragma endregion

#pragma region 関数


#pragma endregion

protected:
	ComPtr<ID3D12PipelineState> pipeline;
	std::array<SpriteVertex, 4> vertices;


#pragma region バッファ

	//インデックスは、スプライトをバラバラにしたりする処理に使えそうだから実装しとく
	//ジオメトリでやるから消した
	VertexBufferSet vertexBufferSet;
	ComPtr<ID3D12Resource> constBuffer;

	static std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	static ComPtr<ID3D12DescriptorHeap> textureHeap;
#pragma endregion

	void CreateBuffer();

	SpriteConstData constData;
#pragma region 関数


	

	void CommonDataMat();
	void SetCmdList(Texture* texture);

#pragma region マップ
	void MapVertexBuffer(void** data);
	void UnmapVertexBuffer();
#pragma endregion


#pragma endregion

public:
	Sprite();
	~Sprite();

	

	virtual void Draw(Texture* texture);

#pragma region 開発者用関数

	static void Initialize(ID3D12Device* dev,ID3D12GraphicsCommandList* cmd);

	static void CreateTextureBuffer(Texture* texture);



#pragma endregion




};

