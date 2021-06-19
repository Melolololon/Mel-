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

//AddColor�Ƃ��܂Ƃ߂悤!

//���f���̍��W�Ȃǂ��܂Ƃ߂�����
class ModelObject
{
private:
	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;
	static PipelineState defaultPipeline;

	
	std::vector<PipelineState*> pPipeline;




	//[���f�����̃I�u�W�F�N�g����]
	std::vector < Material> materials;



protected:

	//�萔�o�b�t�@
	static const int CONST_BUFFER_REGISTER = 0;
	std::vector<ComPtr<ID3D12Resource>> constBuffer;//���C��(��{�I�ȏ��)

	static const int MATERIAL_BUFFER_REGISTER = 2;
	std::vector<ComPtr<ID3D12Resource>> materialConstBuffer;//�}�e���A��

	static const int USER_BUFFER_REGISTER = 1;
	std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//���[�U�[
	ConstBufferData userConstBufferData;

	static const int MODEL_BUFFER_REGISTER = 3;
	std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//���f�����L
	ConstBufferData::BufferType modelConstBufferType = ConstBufferData::BufferType::BUFFER_TYPE_NONE;


	//�萔�ɃZ�b�g������W�Ȃǂ̒l
	//[���f�����̃I�u�W�F�N�g��]
	std::vector < ModelConstData> modelConstDatas;


	//���f��(�N���G�C�g�����犄�蓖�Ă�)
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

