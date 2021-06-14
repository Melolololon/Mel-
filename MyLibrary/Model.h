#pragma once
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>


#include"PipelineState.h"
#include"Texture.h"
#include"RenderTarget.h"


struct BufferData
{
	enum BufferType
	{
		BUFFER_TYPE_NONE,//�������Ȃ�
		BUFFER_TYPE_COMMON,//�S���f������
		BUFFER_TYPE_EACH_MODEL,//�e���f������
		BUFFER_TYPE_EACH_MODEL_OBJECT,//�e���f���e�I�u�W�F�N�g����
	};

	BufferType bufferType;//�o�b�t�@�̐�
	size_t bufferDataSize;//�o�b�t�@1�̃T�C�Y
};

//���f���N���X�Ɍp�����邽�߂̃N���X
class Model
{
public:

	//�q�[�v�����������邽�߂̃^�O
	enum HeapBufferTag
	{
		HEAP_TAG_TEXTURE_BUFFER,//�e�N�X�`��
		HEAP_TAG_COMMON_CONST_BUFFER,//�g���
		HEAP_TAG_LIBRARY_CONST_BUFFER,//���C���̃o�b�t�@
		HEAP_TAG_MATERIAL_CONST_BUFFER,//�}�e���A��
		HEAP_TAG_MODEL_CONST_BUFFER,//�e���f���N���X���Ƃɗp�ӂ���o�b�t�@
		HEAP_TAG_USER_CONST_BUFFER,//���p�҂��p�ӂ����f�[�^�̃o�b�t�@
	};

	

private:
#pragma region �ϐ�
	//�S����
	int commonConstBufferNum = 0;
	//���f������
	int eachModelConstBufferNum = 0;
	//���f���̃I�u�W�F�N�g����
	int eachModelObjectConstBufferNum = 0;
	BufferData::BufferType modelConstBufferType;
	BufferData::BufferType userConstBufferType;

	static ID3D12Device* device;
	static std::vector<ID3D12GraphicsCommandList*>cmdLists;

	//����X�v���C�g�݂����Ƀq�[�v����Ăяo���Ȃ��悤�ɂ��āA
	//1������������悤�ɂ����ق�������?
	//�o�b�t�@1��������āA�r���[�𕡐����悤�ɂ���
	//static ComPtr<ID3D12Resource>commonBuffers;
	static ComPtr<ID3D12RootSignature>rootSignature;

	static DirectX::XMMATRIX viewAndProjectionMat;
	static DirectX::XMMATRIX cameraRotateMat;
	
	static CommonConstData commonConstData;
	static std::vector<Model*> pModels;

#pragma endregion

#pragma region �֐�
	void MapCommonConstData();

#pragma region ����

	void CreateDescriptorHeap
	(
		const int arrayNum
	);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">���f��������</param>
	/// <param name="modelFileObjectNum">���f���t�@�C��(.obj�Ȃ�)�ɉ��I�u�W�F�N�g�����邩�B</param>
	/// <param name="heapTop">�q�[�v�̂ǂ����琶�����邩</param>
	/// <param name="constDataSize">��������o�b�t�@�̍\���̂̃o�C�g��</param>
	/// <param name="tags">��������o�b�t�@�̎��(���т�constDataSize�ƍ��킹�邱��)</param>
	void CreateConstBuffer
	(
		const int modelNum,
		const int modelFileObjectNum,
		const int heapTop,
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void CreateTextureBuffer
	(
		const std::vector<Texture*>& textures,
		const int heapTop
	);

	void Model::CreateOneColorTextureBuffer
	(
		const Color& color,
		const int heapTop
	);

#pragma endregion

#pragma endregion

protected:
#pragma region �ϐ�
	int modelNum;//������
	int modelObjectNum;//�t�@�C�����̃I�u�W�F�N�g��

	//�e�N�X�`���o�b�t�@��
	int textureBufferNum;

	std::vector<ComPtr<ID3D12PipelineState>> pipeline;

	//[obj�̒��̃��f������]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;

	//�C���f�b�N�X
	std::vector<std::vector<USHORT>> indices;

	ComPtr<ID3D12DescriptorHeap>desHeap;

	
	std::vector<std::vector<ComPtr<ID3D12Resource>>> constBuffer;//���C��(��{�I�ȏ��)
	std::vector<std::vector<ComPtr<ID3D12Resource>>> materialConstBuffer;//�}�e���A��
	std::vector<std::vector<ComPtr<ID3D12Resource>>> userConstBuffer;//���[�U�[
	std::vector<std::vector<ComPtr<ID3D12Resource>>> modelConstBuffer;//���f�����L
	


	//�e�N�X�`�����Ɏ�����������R�����g�A�E�g
	//�q�[�v�ɃV�F�[�_�[���\�[�X�r���[����炸�ɕ`�悷��̂��s�\�������̂ŁA�߂���
	std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	
	//�f�B�X�N���v�^�q�[�v�̃o�b�t�@���������邽�߂̂���
	std::vector<HeapBufferTag>heapTags;


	std::vector <Model*> parentModel;
	std::vector <int> parentModelNumbers;


	//�萔�ɃZ�b�g������W�Ȃǂ̒l
	//[���f������][���f�����̃I�u�W�F�N�g��]
	std::vector <std::vector<ModelConstData>>modelConstDatas;
	//[���f�����̃I�u�W�F�N�g����]
	std::vector<Material> materials;

#pragma endregion

#pragma region �֐�

	void DrawCommonProcessing(const int modelNum, const std::string& rtName);
	void MapConstData(const int modelNum, const Camera* camera);
	void SetCmdList(const int modelNum);


#pragma region �o�b�t�@����

#pragma region �����܂Ƃ�
	//CreateModelVertexResources��CreateModelHeapResources�܂Ƃ߂Ă�������

	/// <summary>
	/// �V�F�[�_�[�ɒ��_���𑗂邽�߂̃f�[�^�𐶐����܂��B
	/// (���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�̂���)
	/// </summary>
	/// <param name="verticesSize">���_�f�[�^�̃o�C�g��</param>
	/// <param name="verticesNum">�e</param>
	/// <param name="indicesNum"></param>
	void CreateModelVertexResources
	(
		const size_t verticesSize,
		const std::vector<size_t>& verticesNum,
		const std::vector<std::vector<USHORT>>& indicesNum
	);

	/// <summary>
	/// �V�F�[�_�[�ɒ��_���ȊO�̃f�[�^�𑗂邽�߂̃f�[�^���쐬���܂��B
	/// (�f�B�X�N���v�^�q�[�v�Ƃ����Ƀr���[�𐶐�����o�b�t�@�̂���)
	/// </summary>
	/// <param name="texturePath"></param>
	/// <param name="modelNum"></param>
	/// <param name="modelFileObjectNum"></param>
	/// <param name="userDataSize"></param>
	void CreateModelHeapResourcesSetTexture
	(
		const std::vector<Texture*>& pTextures,
		const int modelNum,
		const int modelFileObjectNum,
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void CreateModelHeapResourcesSelectColor
	(
		const Color& color,
		const int modelNum,
		const int modelFileObjectNum,
		BufferData* modelBufferData,
		BufferData* userBufferData
	);

	void BufferCreatePreparation
	(
		const int modelNum,
		const int modelFileObjectNum,
		const size_t textureNum,
		BufferData* modelBufferData,
		BufferData* userBufferData 
	);
#pragma endregion


	//���쒸�_�g���Ƃ��Ɍp�����Ȃ��悤�ɂ�������A
	//�}�b�v���p�u���b�N�Ɉړ�
	//�p�������邽�߁Aprotected�ɖ߂���
#pragma region Map

#pragma region �}�b�v
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">���f���ԍ�(obj���̂ǂ̃��f����)</param>
	/// <param name="vertexStruct">GPU�̉��z�A�h���X���i�[����|�C���^�̃|�C���^</param>
	void MapVertexBuffer
	(
		const int modelNum,
		void** vertexData
	);

	void UnmapVertexBuffer(const int& modelNum);


	void MapIndexBuffer
	(
		const int modelNum,
		void** index
	);

	void UnmapIndexBuffer(const int& modelNum);

	//���ꂾ�Ɨ��p�҂����C�u�����̃o�b�t�@�������������Ă��܂�
	//�����ƃ}�b�v�����ςȂ��ł������̂ł�?
	//�폜����unmap����΂����̂ł�? 
	//�}�b�v�����ςȂ��ɂ���ꍇ�A�o�b�t�@���ƂɃ|�C���^�p�ӂ��Ȃ��Ƃ����Ȃ�

	//�萔�o�b�t�@�̏��Ԃ͌Œ艻����B
	//create�����͂܂Ƃ߂�

	//enum ConstBufferType
	//{
	//	CONSTBUFFER_LIBRARY,
	//	CONSTBUFFER_MATERIAL,
	//	CONSTBUFFER_USER
	//};
	///// <summary>
	///// 
	///// </summary>
	///// <param name="modelNum">���Ԗڂ̃��f����</param>
	///// <param name="objectNum">���f���̉��Ԗڂ̃I�u�W�F�N�g��</param>
	///// <param name="structNum">�ǂ̃o�b�t�@�̉��z�A�h���X���擾���邩</param>
	///// <param name="constData"></param>
	//void mapConstBuffer
	//(
	//	const int modelNum,
	//	const int objectNum,
	//	const ConstBufferType type,
	//	void** constData
	//);

	//const int objectNum�́A�܂��ʂɃZ�b�g�ł���悤�ɂ������Ȃ����炱�����Ă�
	void MapUserConstBuffer
	(
		const int modelNum,
		void** constData
	);
#pragma endregion

#pragma endregion

#pragma endregion



#pragma region �X���[�Y�V�F�[�f�B���O
	void CalcSmoothingNormal
	(
		std::vector<Vector3> normals
	);
#pragma endregion

#pragma endregion

public:
	Model();
	virtual ~Model();

#pragma region �J���җp�֐�

	static void SetCommonConstData(const CommonConstData& data);


	static void Initialize
	(
		ID3D12Device* dev,
		std::vector<ID3D12GraphicsCommandList*> cmdList
	);

	

	static void SetViewAndProjectionMatrix(const DirectX::XMMATRIX& mat) { viewAndProjectionMat = mat; }
	static void SetCameraRotateMat(const DirectX::XMMATRIX& mat) { cameraRotateMat = mat; }
#pragma endregion

	 

#pragma region ����
	void SetPosition(const Vector3& position, const int modelNum);
	void SetAngle(const Vector3& angle,const int modelNum);
	void SetScale(const Vector3& scale, const int modelNum);
#pragma endregion

#pragma region �`��

	virtual void Draw(const int modelNum,const std::string& rtName = "");
	virtual void AllDraw();
#pragma endregion

#pragma region �p�C�v���C��

	void SetPipeline(PipelineState* pipelineState, const int modelNum);
	void SetPipelineAllSet(PipelineState* pipelineState);

#pragma endregion

	
};

