#pragma once
#include<vector>
#include<array>

#include"Sprite2DBase.h"
#include"DirectXStruct.h"
#include"Vector.h"
#include"PipelineState.h"
#include"Camera.h"

// レンダーターゲットをどこに描画するかを指定できるようにする
// とりあえずレンダーターゲット優先。オブジェクトの描画場所は後回し
// レンダーターゲットに次のレンダーターゲットのポインタを渡すんじゃなくて、
// 描画順を記述して渡すようにする?
// レンダリングする画像,描画対象の配列って感じで指定する?(配列じゃなくて8個変数作る?)
// ポインタ渡す?名前渡す?
// 一回ずつ渡さずに、まとめたのを送ってもらったほうがいい
// 
// 先頭かどうか記録したほうがいい?

// レンダーターゲットのヒープまとめていいかも
// すれば、マルチテクスチャが自由にできる

// スプライトクラスと分離したほうがいいかも
// スプライトクラスでレンダーターゲットインクルードしないとレンダリング先指定できなくなる可能性ある
//それまで、レンダーターゲット複数作成不可


namespace MelLib
{

	class RenderTarget :public Sprite2DBase
	{
	public:
		struct RTDrawData
		{
			//レンダーターゲット
			RenderTarget* rt = nullptr;
			//レンダーターゲットをレンダリングするレンダーターゲット
			RenderTarget* renderingRT = nullptr;
		};

	private:
		static std::vector<RTDrawData> rtDrawData;

		static const int RT_NUM = 1;

		static std::unordered_map<std::string, std::unique_ptr<RenderTarget>>pRenderTargets;
		static UINT createCount;
		static std::string mainRenderTargetNama;

		static float sClearColor[4];
		static PipelineState defaultPipeline;
		static ComPtr<ID3D12RootSignature>rootSignature;

		//現在描画対象になっているレンダーターゲット(マルチ対応のため、配列にしている)
		static RenderTarget* pCurrentSelectRTs;

		ComPtr<ID3D12Resource>textureBuffer[RT_NUM];
		ComPtr<ID3D12DescriptorHeap>descHeap;//テクスチャ(レンダリング結果) + ポストエフェクトの定数バッファビュー

		ComPtr<ID3D12DescriptorHeap>rtvHeap;

		ComPtr<ID3D12Resource>depthBuffer;
		ComPtr<ID3D12DescriptorHeap>depthHeap;

		//カメラのポインタ
		Camera* pCamera = nullptr;
		float clearColor[4];
	public:

		RenderTarget(const Color& color);
		~RenderTarget();

		static void SetRenderingRenderDrawData(const std::vector<RTDrawData>& data) { rtDrawData = data; }

		static void Create(const Color& initColor, const std::string& name = "");
		static void Delete(const std::string& name);
		static RenderTarget* Get(const std::string& name = mainRenderTargetNama) { return pRenderTargets[name].get(); }

		static bool Initialize();

		//これprivateに
		void PreDrawProcess();
		void SetCmdList();
		static void AllDraw();
		//PreDrawProcessを呼び出す関数
		static void DrawBegin();
		static void MainRTDraw();

		static void ChangeCurrentRenderTarget(RenderTarget* pRTs);

		/// <summary>
		/// カメラをセットします。
		/// </summary>
		/// <param name="name"></param>
		void SetCamera(Camera* pCamera) { this->pCamera = pCamera; }


		static const std::string& GetMainRenderTargetNama() { return mainRenderTargetNama; }

		Camera* GetCamera() { return pCamera; }

	};

}