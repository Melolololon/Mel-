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


	static std::unordered_map<std::string,std::unique_ptr<RenderTarget>>pRenderTarget;
	static UINT createCount;

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
	static RenderTarget& GetRenderTarget(std::string name)
	{
		return *pRenderTarget[name];
	}

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

