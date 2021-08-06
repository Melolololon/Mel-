#pragma once
#include<vector>
#include<array>

#include"Sprite2DBase.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

namespace MelLib
{
	class RenderTarget :public Sprite2DBase
	{
	private:

		static const int RT_NUM = 1;

		static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
		static UINT createCount;
		static std::string mainRenderTargetNama;

		static float clearColor[4];
		static PipelineState defaultPipeline;
		static ComPtr<ID3D12RootSignature>rootSignature;

		ComPtr<ID3D12Resource>textureBuffer[RT_NUM];
		ComPtr<ID3D12DescriptorHeap>descHeap;//テクスチャ(レンダリング結果) + ポストエフェクトの定数バッファビュー

		ComPtr<ID3D12DescriptorHeap>rtvHeap;

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
		static void AllDraw();


		void SetCamera(const std::string& name = Camera::GetMainCameraName()) { this->pCamera = Camera::Get(name); }


		static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

		Camera* GetCamera() { return pCamera; }
	};

}