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

	static std::unordered_map<std::string, std::unique_ptr<ModelObject>>pModelObjects;

	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;
	static PipelineState defaultPipeline;


	std::vector<PipelineState*> pPipeline;

	//[モデル内のオブジェクトごと]
	std::vector < Material> materials;


#pragma region ボーンとアニメーションの情報
	static const UINT BONE_MAX = 64;
	struct SkinConstBufferData
	{
		DirectX::XMMATRIX bones[BONE_MAX];
	};

	//fbxモデルのアニメーション用の情報をまとめたもの
	struct FbxAnimationData
	{
		FbxTime freamTime;
		FbxTime startTime;
		FbxTime endTime;
		FbxTime currentTime;
		int animationMag;
	};


	//[ボーンごと]
	std::vector<BoneData>boneDatas;
	std::vector<ParentBoneData> parentBoneDatas;//親ボーンと影響度

	FbxAnimationData fbxAnimationData;

#pragma endregion
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
	std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//モデル特有(アニメーション関係の情報)
	ConstBufferData modelConstBufferData;


	//定数にセットする座標などの値
	//[モデル内のオブジェクト数]
	std::vector<ModelConstData> modelConstDatas;


	//モデル(クリエイトしたら割り当てる)
	ModelData* pModelData = nullptr;
	UINT modelFileObjectNum = 0;

	void CreateConstBuffer();

	void DrawCommonProcessing(const std::string& rtName);
	void MapConstData(const Camera* camera);
	void SetCmdList();


public:
	
	//nullptr渡される可能性を考えると、boolをreturnできるようにしたほうがいい?
	ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData);
	~ModelObject() {}

	static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);

	static bool Create(ModelData* pModelData, ConstBufferData* userConstBufferData,const std::string& name);

	void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

	bool CreateObject(ModelData* pModelData, ConstBufferData* userConstBufferData);

#pragma region 操作見た目変更


	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetAngle(const Vector3& angle);

#pragma endregion

	void SetPipeline(PipelineState* pipelineState);


};

