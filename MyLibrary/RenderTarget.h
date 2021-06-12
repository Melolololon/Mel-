#pragma once
#include<vector>
#include<array>

#include"Sprite2D.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"


//レンダーターゲットクラス
//描画時に渡すことで、渡したレンダーターゲットに描画できるようにする
//セットしない場合は、バックバッファに直接または1枚目のレンダーターゲットに描画

class RenderTarget :public Sprite2D
{
private:

	static std::vector<RenderTarget*>pRenderTarget;
	static float clearColor[4];
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;

	ComPtr<ID3D12Resource>textureBuffer[2];
	ComPtr<ID3D12DescriptorHeap>descHeap;//テクスチャ(レンダリング結果) + ポストエフェクトの定数バッファビュー

	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	ComPtr<ID3D12Resource>depthBuffer;
	ComPtr<ID3D12DescriptorHeap>depthHeap;

	//カメラのポインタ
	Camera* pCamera = nullptr;

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="color">初期化色</param>
	RenderTarget(const Color& color);
	~RenderTarget();

	//void Create(const Color& color);

	static bool Initialize();

	/// <summary>
	/// 描画前の処理
	/// </summary>
	void PreDrawProcess();

	void Draw()override;
	static void AllDraw();

	/// <summary>
	/// レンダーターゲットにカメラをセットします。レンダーターゲットには、セットされたカメラに映っているものが描画されます。
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	void SetCamera(Camera* pCamera){ this->pCamera = pCamera; }

};

