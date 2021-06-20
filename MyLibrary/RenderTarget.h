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

	static float clearColor[4];
	static PipelineState defaultPipeline;
	static ComPtr<ID3D12RootSignature>rootSignature;

	static const UINT RT_NUM = 2;
	static const UINT MOTION_BLAR_RT_NUM = 5;
	//[モーションブラー用レンダーターゲット(前のフレームの結果描画に使用)][同時描画数]
	std::array<std::array<ComPtr<ID3D12Resource>, RT_NUM>, MOTION_BLAR_RT_NUM>textureBuffer;
	ComPtr<ID3D12DescriptorHeap>descHeap;//テクスチャ(レンダリング結果) + ポストエフェクトの定数バッファビュー
	ComPtr<ID3D12Resource>motionBlurConstBuffer;
	int renderingRTNum = 0;
	static std::vector<Color>rtColor;


	ComPtr<ID3D12DescriptorHeap>rtvHeap;

	ComPtr<ID3D12Resource>depthBuffer;
	ComPtr<ID3D12DescriptorHeap>depthHeap;

	//カメラのポインタ
	Camera* pCamera = nullptr;

	//モーションブラーを行うかどうか
	bool isMotionBlur = false;

	void MapMotionBlurConstData();

public:

	RenderTarget(const Color& color);
	~RenderTarget();


	static void Create(const Color& initColor, const std::string& name = "");
	static void Delete(const std::string& name);
	static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

	static bool Initialize();

	void PreDrawProcess();
	void SetCmdList();
	static void AllDraw();



#pragma region セット


	void SetCamera(const std::string& name = Camera::GetMainCameraName()) { this->pCamera = Camera::Get(name); }

	void SetMotionBlurFlag(const bool flag) { isMotionBlur = flag; }

#pragma endregion

#pragma region ゲット

	static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

	Camera* GetCamera() { return pCamera; }
#pragma endregion


};

