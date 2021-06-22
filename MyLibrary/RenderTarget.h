#pragma once
#include<vector>
#include<array>

#include"Sprite2D.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"


class RenderTarget :public Sprite2D
{
private:

	static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
	static UINT createCount;
	static std::string mainRenderTargetNama;

	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;

	static const UINT RT_NUM = 2;
	static const UINT BLUR_RT_NUM = 2;
	//[モーションブラー用レンダーターゲット(前のフレームの結果描画に使用)][同時描画数]
	ComPtr<ID3D12Resource>textureBuffer[BLUR_RT_NUM][RT_NUM];
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;//テクスチャ(レンダリング結果)
	ComPtr<ID3D12DescriptorHeap>rtvDescHeap;
	int renderingRTNum = 0;


	ComPtr<ID3D12Resource>depthBuffer;
	ComPtr<ID3D12DescriptorHeap>depthHeap;

	//カメラのポインタ
	Camera* pCamera = nullptr;



public:

	RenderTarget(const Color& color);
	~RenderTarget();


	static void Create(const Color& initColor, const std::string& name = "");
	static void Delete(const std::string& name);
	static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

	static bool Initialize();

	void PreDrawProcess();
	void SetCmdList();
	void Draw();
	static void AllDraw();



#pragma region セット

	void SetCamera(const std::string& name = Camera::GetMainCameraName()) { this->pCamera = Camera::Get(name); }



#pragma endregion

#pragma region ゲット
	static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

	Camera* GetPCamera(){ return pCamera; }
#pragma endregion


};

