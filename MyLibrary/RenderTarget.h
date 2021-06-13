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


	static std::unordered_map<std::string,std::unique_ptr<RenderTarget>>pRenderTargets;
	static UINT createCount;
	static std::string mainRenderTargetNama;

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


	RenderTarget(const Color& color);
	~RenderTarget();
	
	/// <summary>
	/// レンダーターゲットを生成します。
	/// </summary>
	/// <param name="initColor">初期化色</param>
	/// <param name="name">名前</param>
	static void Create(const Color& initColor, std::string name = "");
	/// <summary>
	/// レンダーターゲットを削除します。
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns></returns>
	static void Delete(std::string name);
	
	/// <summary>
	/// レンダーターゲットを参照します。
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>nameで指定したレンダーターゲットの参照</returns>
	static RenderTarget& Get(std::string name = mainRenderTargetNama)
	{
		return *pRenderTargets[name];
	}

	static bool Initialize();

	/// <summary>
	/// 描画前の処理
	/// </summary>
	void PreDrawProcess();

	void SetCmdList();
	static void AllDraw();

	/// <summary>
	/// レンダーターゲットにカメラをセットします。レンダーターゲットには、セットされたカメラに映っているものが描画されます。
	/// </summary>
	/// <param name="camera">カメラのポインタ</param>
	void SetCamera(Camera* pCamera){ this->pCamera = pCamera; }

	/// <summary>
	/// 設定されているカメラを参照します。
	/// </summary>
	Camera& GetCamera() { return *pCamera; }
};

