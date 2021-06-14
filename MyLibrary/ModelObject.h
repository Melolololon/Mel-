#pragma once
#include"Model.h"

//AddColor�Ƃ��܂Ƃ߂悤!

class ModelObject
{
private:
	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;
	static ComPtr<ID3D12RootSignature>rootSignature;


	//���f��(�N���G�C�g�����犄�蓖�Ă�)
	Model* model = nullptr;
	int modelFileObjectNum = 0;
	
	std::vector<PipelineState*> pPipeline;


	ID3D12DescriptorHeap* textureDescHeap;

	//�萔�o�b�t�@
	static const int CONST_BUFFER_REGISTER = 0;
	std::vector<ComPtr<ID3D12Resource>> constBuffer;//���C��(��{�I�ȏ��)


	static const int MATERIAL_BUFFER_REGISTER = 2;
	std::vector<ComPtr<ID3D12Resource>> materialConstBuffer;//�}�e���A��

	static const int USER_BUFFER_REGISTER = 1;
	std::vector<ComPtr<ID3D12Resource>> userConstBuffer;//���[�U�[
	BufferData::BufferType userConstBufferType = BufferData::BufferType::BUFFER_TYPE_NONE;

	static const int MODEL_BUFFER_REGISTER = 3;
	std::vector<ComPtr<ID3D12Resource>> modelConstBuffer;//���f�����L
	BufferData::BufferType modelConstBufferType = BufferData::BufferType::BUFFER_TYPE_NONE;

	//�萔�ɃZ�b�g������W�Ȃǂ̒l
	//[���f������][���f�����̃I�u�W�F�N�g��]
	std::vector < ModelConstData> modelConstDatas;
	//[���f�����̃I�u�W�F�N�g����]
	std::vector < Material> materials;



protected:

	void CreateConstBuffer
	(
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void DrawCommonProcessing(const std::string& rtName);
	void MapConstData(const Camera* camera);
	void SetCmdList();


public:
	ModelObject(){}
	~ModelObject(){}
	
	static bool Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList);

	virtual void Draw(const std::string& rtName = RenderTarget::GetMainRenderTargetNama());

	void SetPosition(const Vector3& position);
	void SetScale(const Vector3& scale);
	void SetAngle(const Vector3& angle);
	
	void SetPipeline(PipelineState* pipelineState);
};

