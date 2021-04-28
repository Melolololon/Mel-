#pragma once
#include"DirectXStruct.h"

#include<d3d12.h>
#include<dxgi.h>
#include<vector>

//これを継承して2D、3Dスプライトクラスを作る
//Drawは仮想関数。それぞれにmap処理を書く
//

class Sprite
{
private:

	static DirectX::XMMATRIX sprite2DCameraMatrix;
	static DirectX::XMMATRIX sprite3DCameraMatrix;

	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;

	

	//インデックスは、スプライトをバラバラにしたりする処理に使えそうだから実装しとく
	VertexBufferSet vertexBufferSet;
	IndexBufferSet indexBufferSet;

	//全スプライトでテクスチャヒープ共有する(ここからシェーダーリソースビュー引っ張りだす)
	static ComPtr<ID3D12DescriptorHeap>desHeap;
	ComPtr<ID3D12Resource> constBuffer;
	//スプライト全体でバッファ共有する
	static std::vector<ComPtr<ID3D12Resource>>textureBuffer;

	std::string modelClassName;
protected:
	ComPtr<ID3D12PipelineState> pipeline;
	ID3D12PipelineState* currentSetPipeline;

public:
	Sprite();
	~Sprite();

#pragma region 開発者用関数

	
#pragma endregion


#pragma region 見た目操作
	//AddColorとか
#pragma endregion

	/// <summary>
	/// スプライトを描画します。
	/// </summary>
	virtual void Draw();
};

