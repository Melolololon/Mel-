#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"ModelData.h"
#include"PipelineState.h"
#include"Texture.h"
#include"Camera.h"
#include"RenderTarget.h"
#include"BufferData.h"

//AddColorとかまとめよう!

//モデルの座標などをまとめたもの
class ModelObject
{
private:
	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;
	static PipelineState defaultPipeline;

	
	std::vector<PipelineState*> pPipeline;




	//[モデル内のオブジェクトごと]
	std::vector < Material> materials;



protected:

	//定数バッファ
	static const int CONST_BUFFER_REGISTER = 0;
	std::vector<ComPtr<ID3D12Resource>> constBuffer;//メイン(基本的な情報)

	static const int MATERIAL_BUFFER_REGISTER = 2;
	std::vector<ComPtr<ID3D12Resource>> materialConstBuffer;//マテリアル

	static const int USER_BUFFER_REGISTER = 1;
	std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//ユーザー
	ConstBufferData userConstBufferData;

	static const int MODEL_BUFFER_REGISTER = 3;
	std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//モデル特有
	ConstBufferData::BufferType modelConstBufferType = ConstBufferData::BufferType::BUFFER_TYPE_NONE;


	//定数にセットする座標などの値
	//[モデル内のオブジェクト数]
	std::vector < ModelConstData> modelConstDatas;


	//モデル(クリエイトしたら割り当てる)
	ModelData* pModelData = nullptr;
	UINT modelFileObjectNum = 0;

	void CreateConstBuffer
	(
		ConstBufferData* modelBufferData
	);

	void DrawCommonProcessing(const std::string& rtName);
	void MapConstData(const Camera* camera);
	void SetCmdList();


public:
	ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData);
	~ModelObject(){}
	
	static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);

	virtual void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());


	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetAngle(const Vector3& angle);
	
	void SetPipeline(PipelineState* pipelineState);


};

