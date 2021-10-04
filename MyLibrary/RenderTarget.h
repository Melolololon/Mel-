#pragma once
#include<vector>
#include<array>

#include"Sprite2DBase.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

//レンダーターゲットをどこに描画するかを指定できるようにする

namespace MelLib
{
	class RenderTarget :public Sprite2DBase
	{
	private:

		static const int RT_NUM = 1;

		static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
		static UINT createCount;
		static std::string mainRenderTargetNama;

		static float sClearColor[4];
		static PipelineState defaultPipeline;
		static ComPtr<ID3D12RootSignature>rootSignature;

		//現在描画対象になっているレンダーターゲット(マルチ対応のため、配列にしている)
		static std::vector<RenderTarget*> pCurrentSelectRTs;

		ComPtr<ID3D12Resource>textureBuffer[RT_NUM];
		ComPtr<ID3D12DescriptorHeap>descHeap;//テクスチャ(レンダリング結果) + ポストエフェクトの定数バッファビュー

		ComPtr<ID3D12DescriptorHeap>rtvHeap;

		ComPtr<ID3D12Resource>depthBuffer;
		ComPtr<ID3D12DescriptorHeap>depthHeap;

		//カメラのポインタ
		Camera* pCamera = nullptr;
		RenderTarget* pRenderingRenderTarget = nullptr;
		float clearColor[4];
	public:

		RenderTarget(const Color& color);
		~RenderTarget();


		static void Create(const Color& initColor, const std::string& name = "");
		static void Delete(const std::string& name);
		static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

		static bool Initialize();

		//これprivateに
		void PreDrawProcess();
		void SetCmdList();
		static void AllDraw();
		//PreDrawProcessを呼び出す関数
		static void AllDrawBegin();

		static void ChangeCurrentRenderTarget(std::vector<RenderTarget*> pRTs);

		/// <summary>
		/// カメラをセットします。
		/// </summary>
		/// <param name="name"></param>
		void SetCamera(Camera* pCamera) { this->pCamera = pCamera; }

		/// <summary>
		/// レンダーターゲットを描画するレンダーターゲットを指定します。
		/// </summary>
		/// <param name="pRT">レンダーターゲットのポインタ。指定しない場合、メインレンダーターゲットに描画します。</param>
		void SetRenderingRenderTarget(RenderTarget* pRT = RenderTarget::Get(mainRenderTargetNama)) { pRenderingRenderTarget = pRT; }

		static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

		Camera* GetCamera() { return pCamera; }

	};

}