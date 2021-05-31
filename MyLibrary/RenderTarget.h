#pragma once
#include<vector>
#include<array>

#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

//レンダーターゲットクラス
//描画時に渡すことで、渡したレンダーターゲットに描画できるようにする
//セットしない場合は、バックバッファに直接または1枚目のレンダーターゲットに描画

class RenderTarget
{
private:
	static std::vector<RenderTarget*>pRenderTarget;
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;
	
	VertexBufferSet vertexBufferSet;

	std::array<Vertex, 4>vertices;

	ComPtr<ID3D12DescriptorHeap>descHeap;//テクスチャ(レンダリング結果) + ポストエフェクトの定数バッファビュー
	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	//カメラのポインタ
	Camera* pCamera;
public:
	RenderTarget(){}
	~RenderTarget(){}

	static bool Initialize(ID3D12Device* dev,ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// レンダーターゲットを生成します。
	/// </summary>
	bool CreateRenderTarget();

	
	/// <summary>
	/// レンダーターゲットにカメラをセットします。レンダーターゲットには、セットされたカメラに映っているものが描画されます。
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	void SetCamera(Camera* pCamera){ this->pCamera = pCamera; }

};

