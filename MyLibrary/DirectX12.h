#pragma once
#include<Windows.h>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<random>
#include<ctime>
#include<string>
#include<unordered_map>
#include<wrl.h>


#include"DirectInput.h"
#include"CreateBuffer.h"
#include"CreatePolygon.h"
#include"CreatePipeline.h"
#include"ModelLoader.h"
#include"CreateCamera.h"
#include"Vector.h"
#include"DirectXStruct.h"
#include"DirectXTexLoader.h"

#define DIRECTINPUT_VERSION 0x0800
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<dxgidebug.h>
#include<dinput.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



#include"Model.h"
#include"PrimitiveModel.h"
#include"ObjModel.h"
#include"FbxModel.h"
#include"Sprite.h"
#include"Sprite2D.h"
#include"Sprite3D.h"

enum LibraryPipeline
{
	PIPELINE_NOT_DEPTH_TEST,//�[�x�e�X�g����(z�ɂ�����炸�A�֐��̌Ăяo�������Ԃŕ`�悳��܂�)
	PIPELINE_CULL_NONE,//�J�����O�Ȃ�(���f���̗������`�悵�܂�)
	PIPELINE_NO_WRITE_ALPHA,//�����������������܂Ȃ�
	PIPELINE_NORMAL,
	PIPELINE_OBJ_ANIMATION,
};


using namespace Microsoft::WRL;

class DirectX12 final
{

	enum DrawType
	{
		none,
		box,
		circle,
	};

	enum Dimension
	{
		dimention2D,
		dimention3D
	};

private:
	float clearColor[4];

#pragma region ���݂̕`��R�}���h���
	enum RSState
	{
		RS_MODEL,
		RS_SPRITE,
		RS_POINT,
	};
#pragma endregion

	DirectX::XMMATRIX cameraMat;

#pragma region Windows
	HWND hwnd;
	int winWidth;
	int winHeight;
#pragma endregion

#pragma region ��{�I�ȃI�u�W�F�N�g

	//DirectX�ϐ�
	HRESULT result;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;


#pragma region RTV

	//���C��
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Resource> backBuffer[2];
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;

#pragma endregion

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	D3D12_RESOURCE_BARRIER barrierDesc;


	ComPtr<ID3D12RootSignature> rootSignature;
	ComPtr<ID3D12RootSignature> spriteRootsignature;
	ComPtr<ID3D12RootSignature> pointRootsignature;

	//���[�U�[�쐬�̃p�C�v���C��
	//��̒P�̂̂���������ɂ܂Ƃ߂�?
	//vector����Ȃ��ĕ��ʂ̔z��ɂ���?
	std::vector<ComPtr<ID3D12PipelineState>> pipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> spritePipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> sprite3DPipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> pointPipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> objPipelineStates;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC spriteGpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC sprite3DGpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pointGpipeline;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;
	int startPipelineCreateNum;//Initialize���ɍ��3D�̃p�C�v���C����

	int pipelineNum;
	int spritePipelineNum;
	int sprite3DPipelineNum;


#pragma endregion

#pragma region �o�b�t�@

#pragma region ���_ �C���f�b�N�X
	//[���f������][obj�̒��̃��f������]
	std::unordered_map<std::string, std::vector<VertexBufferSet>> vertexBufferSet;
	std::unordered_map<std::string, std::vector<IndexBufferSet>>indexBufferSet;

	std::vector<VertexBufferSet> spriteVertexBuffSet;
	std::vector<IndexBufferSet> spriteIndexBufferSet;//����Œ肾���畡������Ȃ�?

	std::vector< std::vector<VertexBufferSet>> pointVertexBuffSet;
#pragma endregion

#pragma region �q�[�v�ɓ����o�b�t�@

	std::unordered_map<std::string, std::vector<ConstBufferSet>> constBufferSet;
	std::unordered_map<std::string, ComPtr<ID3D12Resource>>commonBuffers;
	std::unordered_map<std::string, TextureBufferSet> textureBufferSet;


	std::vector<ConstBufferSet> spriteConstBufferSet;
	std::vector<TextureBufferSet> spriteTextureBufferSet;

	std::vector<ConstBufferSet> pointConstBufferSet;
	std::vector<TextureBufferSet> pointTextureBufferSet;

	std::vector<TextureBufferSet>spriteFontTextureBufferSet;

#pragma endregion

	DepthBufferSet depthBufferSet;
#pragma endregion

#pragma region �q�[�v

	std::unordered_map<std::string, ComPtr<ID3D12DescriptorHeap>> basicHeaps;
	ComPtr<ID3D12DescriptorHeap> spriteHeap;
	ComPtr<ID3D12DescriptorHeap> pointHeap;
	ComPtr<ID3D12DescriptorHeap> spriteFontHeap;
	ComPtr<ID3D12DescriptorHeap> depthHeap;


#pragma endregion

	//����p�C�v���C���쐬���Ɏg�p����desc
	D3D12_INPUT_ELEMENT_DESC* userDesc;

#pragma region �o�b�t�@�쐬�Ȃǂ̃N���X

	CreatePolygon* createPolygon;
	CreateBuffer* createBuffer;
	CreatePipeline* CreatePipelineState;
	CreateCamera* mainCamera;

#pragma endregion


#pragma region ���_

#pragma region 3D
	//[���f������(key�ŃA�N�Z�X)][obj�ɂ��郂�f������][���_����]
	std::unordered_map<std::string, std::vector<std::vector<Vertex>>> vertices;
	//std::unordered_map<std::string, std::vector<std::vector<OBJAnimationVertex>>> objAnimationVertices;//Vertex�ɉ����A�{�[���ԍ���ǉ�



	std::unordered_map<std::string, std::vector<std::vector<USHORT>>> indices;
#pragma endregion

	std::vector<std::vector<Vertex>> spriteVertices;
	std::vector<std::vector<unsigned short>> spriteIndices;
	std::vector<std::vector<PointVertex>>pointVertices;//[�_�̔ԍ�����][�_����]
	PointVertex* pointVertexMapData;

	//�X���[�X�V�F�[�f�B���O�p�f�[�^
	std::unordered_map<std::string, std::vector<std::vector<DirectX::XMFLOAT3>>> smoothNormal;//�@���𕽋ς������_�f�[�^

	//[obj���̃I�u�W�F�N�g��]�X���[�Y�V�F�[�f�B���O�̌v�Z�p�f�[�^
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//�X���[�X�V�F�[�f�B���O�p�֐�
	/// <summary>
	/// �X���[�X�V�F�[�f�B���O���s�����߂ɖ@�����v�Z���܂�
	/// </summary>
	void calcSmoothingNormals(const std::string key);


#pragma endregion


#pragma region �I�u�W�F�N�g���

	std::unordered_map < std::string, std::vector<ModelConstData>>modelConstData;

#pragma region �{�[��

	//[key ���f������][�I�u�W�F�N�g�̔ԍ�����][�{�[������]
	std::unordered_map < std::string, std::vector<std::vector<BoneData>>> boneData;


	////���f�����ƂɈႤ�f�[�^(�������f���������炢������)
	//[���f������(key�ŃA�N�Z�X)][.obj���̃I�u�W�F�N�g����][���_����]
	std::unordered_map<std::string, std::vector<std::vector<int>>> objBoneNums;//�{�[���ԍ�
	//[���f������(key�ŃA�N�Z�X)][�{�[������]
	std::unordered_map<std::string, std::vector<DirectX::XMFLOAT3>> objBonePositions;//�{�[���ԍ�



	//struct ModelBoneData
	//{
	//	std::vector<DirectX::XMFLOAT3>bonePos;//�{�[������
	//	//DirectX::XMFLOAT3 rotatePoint;//��]����Ƃ��̊���W
	//};
	//std::unordered_map<std::string, ModelBoneData>modelBoneData;

	
	//[���f������][�{�[������](�I�u�W�F�N�g���Ƃɕς���K�v�Ȃ������Ȃ̂ŁA�������Ă�)
	std::unordered_map<std::string, std::vector<ParentBoneData>> parentBoneData;

#pragma endregion

	std::unordered_map<std::string, std::vector<Material>>materials;



	//std::vector<DirectX::XMVECTOR> spritePosition;
	std::vector<DirectX::XMFLOAT2> spriteScale;
	std::vector<DirectX::XMFLOAT3> spriteAngle;



#pragma endregion


#pragma region �q�[�v�֌W

#pragma region �萔�o�b�t�@�֌W

	//�萔�o�b�t�@�f�[�^
	ModelConstBufferData* constData3D;
	//����
	CommonConstData* commonConstData3D;
	//���[�U�[
	void** userConstData3D;
	unsigned int userConstDataSize3D;
	//�}�e���A���p
	MaterialConstData* materialData;

	//���Ȃ���
	std::vector<ModelConstBufferData*>spriteConstBufferDatas;

	//�_�p�̍\����
	PointConstBufferData* constDataPoint;

	//�萔�o�b�t�@�̃^�O(���[�g�p�����[�^�[�Z�b�g�p)
	//"Texture" �V�F�[�_�[���\�[�X�r���[ t0
	//"LibraryConst" ���C�u�����\���� b0
	//"UserConst" ���[�U�[�\���� b1
	//"Material" �}�e���A���\���� b2
	enum ConstBufferTag
	{
		TEXTURE_BUFFER,
		COMMON_CONST_BUFFER,
		LIBRARY_CONST_BUFFER,
		USER_CONST_BUFFER,
		MATERIAL_CONST_BUFFER,
		OBJ_BONE_MATRIX_CONST_BUFFER
	};
	std::unordered_map<std::string, std::vector<ConstBufferTag>>heapTags;

#pragma endregion

#pragma region �e�N�X�`���֌W
	//�摜�f�[�^
	std::unordered_map<std::string, DirectX::TexMetadata> textureData;
	std::vector<DirectX::TexMetadata> spriteTextureData;

#pragma endregion


#pragma endregion



	//�擾�����X�v���C�g�̃|�C���^
	std::vector<int*>spriteP;
	std::vector<int*>pointP;

	//���ꂢ��Ȃ�
	std::vector<DirectX::XMFLOAT2>spriteSizes;

#pragma region �J�E���g�ϐ�


#pragma region ����ȊO


	int loadTextureCounter;
	int createSpriteCounter;
	int spriteFontDrawCounter;

#pragma endregion

#pragma endregion


#pragma region �J����
	CameraData mainCameraData;


	DirectX::XMFLOAT3 eyeVelocity;
	DirectX::XMFLOAT3 targetVelocity;


#pragma endregion

#pragma region ���C�g

	DirectX::XMFLOAT3 lightVector;//���C�g�̃x�N�g��
	DirectX::XMFLOAT3 lightColor;
#pragma endregion

#pragma region ������
	bool isPlane;//���ʉ����邩

	bool isBillBoardX;
	bool isBillBoardY;
	bool isBillBoardZ;

	bool smoothingFlag;
#pragma endregion


#pragma region �X�v���C�g�t�H���g
	const int SPRITEFONT_MAX = 300;
	std::vector<SpriteFontData>spriteFontData;
#pragma endregion

#pragma region �e�q�\��

	//�I�u�W�F�N�g�̐e��ۑ�����z��
	std::unordered_map < std::string, std::vector < std::string >> parentHeaps;
	std::unordered_map < std::string, std::vector<int>>parentNums;
#pragma endregion

#pragma region �|�X�g�G�t�F�N�g
	//�J�����̉e����^���邩�ǂ���
	bool postEffectCametaFlag;

	//�|�X�g�G�t�F�N�g�p���\�[�X(�����_�[�^�[�Q�b�g���e�N�X�`��)
	std::vector<ComPtr<ID3D12Resource>>postEffectResources;

	//�|�X�g�G�t�F�N�g�p�q�[�v
	//�����_�[�^�[�Q�b�g���Ƃɗp�ӂ����ق����悳����(����\���̃o�b�t�@�̂����Œ�����ς�����)
	ComPtr<ID3D12DescriptorHeap>postEffectHeap;//�e�N�X�`�� + �|�X�g�G�t�F�N�g���̒萔�o�b�t�@�r���[
	ComPtr<ID3D12DescriptorHeap>postEffectRTVHeap;

	//�����_�����O��|�����
	std::vector<Vertex> postEffectVertex;
	VertexBufferSet postEffectVertexBufferSet;
	std::vector<ComPtr<ID3D12PipelineState>>postEffectPipeline;
	ComPtr<ID3D12RootSignature>postEffectRootSigneture;

	//������
	int currentPostEffectPipeline;

	//�l�i�[�p
	std::vector<WorldMatData>postEffectWorldMatData;

	//Map�p
	PostEffectConstData* postEffectConstDataP;
	std::vector<ConstBufferSet>postEfectConstBuffers;//�s��Ƃ�����p

#pragma endregion

#pragma region 2���ڂ̃|�X�g�G�t�F�N�g�p�����_�[�^�[�Q�b�g(��)
	//�|�X�g�G�t�F�N�g�p���\�[�X(�����_�[�^�[�Q�b�g���e�N�X�`��)
	std::vector<ComPtr<ID3D12Resource>>toonShaderResources;

	//�|�X�g�G�t�F�N�g�p�q�[�v
	ComPtr<ID3D12DescriptorHeap>toonShaderHeap;
	ComPtr<ID3D12DescriptorHeap>toonShaderRTVHeap;

	//�����_�����O��|�����
	//���_�ƃ��[�g�V�O�l�`���͎g���܂킷
	std::vector<ComPtr<ID3D12PipelineState>>toonShaderPipeline;
#pragma endregion



#pragma region private�֐�
	void ResizeObjectData(int objectNum, const std::string& key);

	void createHeap();

#pragma region �萔�o�b�t�@

	//���ʃo�b�t�@�쐬������createConstBuffer�݂�����
	//�D���ȃq�[�v�ɍ쐬�ł���悤�ɂ���

	/// <summary>
	/// ���ʃo�b�t�@�쐬
	/// </summary>
	/// <param name="texNum">�e�N�X�`���o�b�t�@��</param>
	/// <param name="key"></param>
	void CreateCommonBuffer
	(
		const int& texNum,
		const std::string& key
	);

	/// <summary>
	/// �萔�o�b�t�@�쐬
	/// </summary>
	/// <param name="heap"></param>
	/// <param name="heapHandleNum"></param>
	/// <param name="cBufferSet"></param>
	/// <param name="constData"></param>
	/// <param name="constDataSize"></param>
	/// <returns></returns>
	bool CreateConstBuffer
	(
		ComPtr<ID3D12DescriptorHeap>& heap,
		const int& heapHandleNum,
		ConstBufferSet* cBufferSet,
		void** constData,
		const UINT& constDataSize
	);

#pragma endregion

#pragma region map����
	void CalcBillboardMat(DirectX::XMMATRIX& matWorld);
#pragma endregion


	/// <summary>
	/// ���C�u�������p�C�v���C�����쐬���܂�
	/// </summary>
	void Create3DObjectPipeline();
#pragma endregion




	DirectX12();
	~DirectX12();

public:

	DirectX12(DirectX12& d) = delete;
	DirectX12 operator=(DirectX12& d) = delete;
	static DirectX12* GetInstance();

#pragma region �������Ȃǂ̕K�{����

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(HWND hwnd, int windouWidth, int windowHeight);

	/// <summary>
	/// �`��R�}���h�Ȃǂ��Ăяo���O�ɌĂԏ���
	/// </summary>
	void LoopStartProcess();

	//�`�揈���B�R�}���h���X�g�̖��߂����s�����肷��
	void LoopEndProcess();

	void Finalize();
#pragma endregion


	//��ʂ̐F�󂯎��
	void SetScreenColor(Color screenColor);

	//�X�v���C�g�̃T�C�Y�擾
	DirectX::XMFLOAT2 GetTextureSize(int textureHandle);


	//���ꂢ���
#pragma region �p�C�v���C���̐ݒ�

	void SetDespTestFlag(bool flag);

#pragma endregion

#pragma region ���[�U�[�p�C�v���C���֌W

	int getStartPipelineCreateNum();

	/// <summary>
	/// ����V�F�[�_�[���g���ăp�C�v���C�������܂�
	/// </summary>
	/// <param name="vShaderData"></param>
	/// <param name="gSyaderData"></param>
	/// <param name="pShaderData"></param>
	bool CreateUserPipelineState(
		PipelineData pipelineData,
		ShaderData vShaderData,
		ShaderData gSyaderData,
		ShaderData pShaderData,
		bool useUserInputLayout);

	/// <summary>
	/// �p�C�v���C����؂�ւ��܂�
	/// </summary>
	/// <param name="num"></param>
	void setPipelineNumber(int num);

	//���C�u�������̒萔�f�[�^���g���ꍇ�Aconst char*�̕�������Ăяo���ēn��
	void setConstMapData(void** dataP, unsigned int dataSize);

	//���[���h�A�r���[�A�v���W�F�N�V�����s�����Z�������̂��󂯎��܂�
	void GetMatrix(float matrix[4][4], const std::string& key, int number);

	//���[���h�͏�Z���Ȃ�
	void GetCameraMatrix(float matrix[4][4]);

	/// <summary>
/// �C���v�b�g���C�A�E�g�̐ݒ�����܂�
/// </summary>
/// <param name="semantics">�Z�}���e�B�N�X��</param>
/// <param name="num">�f�[�^��(Vector3�̏ꍇ�A�O�����Ȃ̂�3�����)</param>
	void SetInputLayout(const std::vector<InputLayoutData>& inputLayoutData);

	/// <summary>
/// ���݂̃C���v�b�g���C�A�E�g���폜���܂�
/// </summary>
	void DeleteInputLayout();


#pragma region �|�X�g�G�t�F�N�g
	void SetPostEffectPipeline(const int& num);

	bool CreateUserPostEffectPipelineState(const ShaderData& pShaderData);
#pragma endregion


#pragma endregion

#pragma region �o�b�t�@�쐬
	void CreatePoint(int createNum, int* point);

	//type�ɉ����Ăǂ�count��++���邩���߂�
	void CreatePolygonData
	(
		const std::vector<Vertex>& vertex, 
		const std::vector<USHORT>& index,
		const std::string& key
	);

	void CreateHeapData
	(
		bool setConstDataFlag,
		const std::string& key,
		const int& objectNumber,
		const wchar_t* texPath,
		const Color* color
	);

#pragma region ���[�U�[
	/// <summary>
	/// ���C�u�����̍\���̂��g�p���A�����Œ��_����ݒ肵�ăI�u�W�F�N�g�𐶐����܂�
	/// </summary>
	/// <param name="vertex"></param>
	/// <param name="p"></param>
	void AddUserVertex(std::vector<Vector3>& vertexPos, std::vector<Vector2>& vertexUV, const std::string& key);


	void AddUserIndex(std::vector<unsigned short>& index, const std::string& key);

	/// <summary>
	/// 
	/// ����̃f�[�^���g�p���A�����Œ��_����ݒ肵�ăI�u�W�F�N�g�𐶐����܂�
	/// </summary>
	/// <param name="vertexData"></param>
	/// <param name="vertexDataSize"></param>
	/// <param name="index"></param>
	/// <param name="polyData"></param>
	void CreateUserPolygon
	(
		void** vertexData,
		UINT vertexDataSize,
		UINT vertexSumDataSize,
		std::vector<USHORT>&index,
		const std::string& key
	);
#pragma endregion

#pragma region ���f��
	VertexType LoadObjVertex
	(
		const char* path,
		bool loadUV,
		bool loadNormal,
		std::string* materialFireName,
		const std::string& key
	);

	void LoadObjMaterial
	(
		std::string materialDirectoryPath,
		std::string materialFileName,
		bool setConstDataFlag,
		const std::string& key,
		const int& objectNumber,
		const VertexType& vType
	);

	//���ꂢ��Ȃ�
	//void loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, PolygonData vertData, HeapData heapData);
#pragma endregion


	void LoadSpriteFont(const wchar_t *const texturePath,const DirectX::XMFLOAT2& lineNum);

	void LoadTexture(const wchar_t* texturePath, Color color);

	//font�́Acreate������̂��X�v���C�g�t�H���g���ǂ���
	void CreateSprite(int* spr, bool font);


#pragma endregion

#pragma region �`��
	//�`�掞����Ȃ��č��W�Ƃ��Z�b�g�����Ƃ��ɖ���}�b�v����?
//��������Ə������x���Ȃ�?
	void SetCmdList(const ModelData& modelData, int number);
	void DataMap(const ModelData& modelData, int number);

	void SpriteSetCmdList(int spriteNum, int textureNum,const bool& sprite3DFlag);

	void SpriteDataMap(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, int spriteNum, int textureNum);
	void Sprite3DDataMap
	(
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT2& size ,
		const int& spriteNumber,
		const int& textureNumber
	);

	void PointSetCmdList
	(
		DirectX::XMFLOAT3 pos, 
		int pointNum, 
		int textureNum, 
		int num
	);

#pragma endregion

#pragma region �폜
	void DeletePolygonData(const ModelData& m);
	void DeleteHeapData(const ModelData& m);

	void DeleteSprite(int sprite);
#pragma endregion

#pragma region �J����
	void SetCameraDataMatrixPoint(Vector3 eye, Vector3 target, Vector3 up);
	void SetCameraData(Vector3 eye, Vector3 target, Vector3 up);

	void SetCameraVelocity(DirectX::XMFLOAT3 eyeVelocity, DirectX::XMFLOAT3 targetVelocity);

	void SetCameraAngre(DirectX::XMFLOAT3 eyeAngle, DirectX::XMFLOAT3 targetAngle, DirectX::XMFLOAT3 upAngle);


	void SetNearAndFar(float nearNum, float farNum);
#pragma endregion

#pragma region �����ڑ���

#pragma region �X���[�X�V�F�[�f�B���O
	void SetSmoothingFlag(bool flag);
#pragma endregion


	void SetMulColor(Color color, const std::string& key, int number);
	void SetAddColor(Color color, const std::string& key, int number);
	void SetSubColor(Color color, const std::string& key, int number);

	void SetIsPlane(bool flag);
	void SetIsBillboard(bool x, bool y, bool z);

	void SetSpriteMulColor(Color color, int spriteNum);
	void SetSpriteAddColor(Color color, int spriteNum);
	void SetSpriteSubColor(Color color, int spriteNum);



	void SetPointMulColor(Color color, int pointNum, int num);



#pragma endregion

#pragma region ���C�g����
	void SetLightVector(DirectX::XMFLOAT3 vector);

	void SetLightColor(Color lightColor);
#pragma endregion

#pragma region ����֐�
	void SetObjectPosition(DirectX::XMFLOAT3 position, const std::string& key, int number);
	void SetObjectScale(DirectX::XMFLOAT3 angle, const std::string& key, int number);
	void SetObjectAngle(DirectX::XMFLOAT3 angle, const std::string& key, int number);
	void SetObjectPushNum(float objectEX, const std::string& key, int number);

	void SetSpritePosition(DirectX::XMFLOAT2 position, int spriteNum);
	void SetSpriteScale(DirectX::XMFLOAT2 scale, int spriteNum);
	void SetSpriteAngle(const DirectX::XMFLOAT3& angle,const int& spriteNum);

	void ChangeSpriteSize(DirectX::XMFLOAT2 size, int spriteData);


	void SetPointScale(DirectX::XMFLOAT2 scale, int pointNum, int num);



#pragma region �|�X�g�G�t�F�N�g

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̍��W��ύX���܂�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rtNum">�ǂ̃����_�[�^�[�Q�b�g���w�肷�邩(���͈Ӗ��Ȃ�)</param>
	void SetRenderTargerPosition(const DirectX::XMFLOAT3& pos, const int& rtNum);

	void SetRenderTargetAngle(const DirectX::XMFLOAT3& angle, const int& rtNum);

	void SetRenderTargetScale(const DirectX::XMFLOAT3& scale, const int& rtNum);

	void SetPostEffectCameraFlag(const bool& flag, const int& rtNum);
#pragma endregion

#pragma region �A�j���[�V����

	std::vector<DirectX::XMFLOAT3> GetBonePosition(const std::string& key);

	void SetObjModelRotatePoint
	(
		const DirectX::XMFLOAT3& position,
		const UINT& boneNum,
		const ModelData& modelData
	);

	/// <summary>
   /// �{�[���������ʒu����ǂ̂��炢�����������w�肵�A�������܂�
   /// </summary>
   /// <param name="vector">�ړ���</param>
   /// <param name="boneNum">���삷��{�[���ԍ�</param>
   /// <param name="key">�L�[</param>
	void SetObjBoneMoveVector
	(
		const DirectX::XMFLOAT3& vector,
		const UINT& boneNum,
		const std::string& key,
		const UINT& objectNum
	);

	/// <summary>
	/// �{�[���ɑΉ����Ă��镔�����ǂ̂��炢�g�傳���邩���w�肵�A�g�債�܂�
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="boneNum"></param>
	/// <param name="key"></param>
	void SetObjBoneScale
	(
		const DirectX::XMFLOAT3& scale,
		const UINT& boneNum,
		const std::string& key,
		const UINT& objectNum
	);

	/// <summary>
	/// �{�[���ɑΉ����Ă��镔�����ǂ̂��炢��]�����邩���w�肵�A��]���܂�
	/// </summary>
	/// <param name="angle"></param>
	/// <param name="boneNum"></param>
	/// <param name="key"></param>
	void SetObjBoneAngle
	(
		const DirectX::XMFLOAT3& angle,
		const UINT& boneNum,
		const std::string& key,
		const UINT& objectNum
	);

	/// <summary>
/// �{�[���ɐe�{�[�����Z�b�g���܂�
/// </summary>
/// <param name="boneNum">�{�[���ԍ�</param>
/// <param name="parentBoneNum">�e�{�[���ԍ�</param>
/// <param name="modelData">���f���f�[�^</param>
	void SetParentObjBone
	(
		const UINT& boneNum,
		const UINT& parentBoneNum,
		const std::string& key
	);

	void SetParentObjBoneScaleImpact
	(
		const UINT& boneNum,
		const DirectX::XMFLOAT3& scaleImpact,
		const std::string& key
	);
	void SetParentObjBoneAngleImpact
	(
		const UINT& boneNum,
		const DirectX::XMFLOAT3& angleImpact,
		const std::string& key
	);
	void SetParentObjBoneMoveVectorImpact
	(
		const UINT& boneNum,
		const DirectX::XMFLOAT3& moveVectorImpact,
		const std::string& key
	);

#pragma endregion


#pragma endregion

#pragma region �����W�擾
	std::vector<std::vector<DirectX::XMFLOAT3>> GetModelVerticesPosition(const std::string& key);

	bool OverrideWriteVertexPosition(std::vector<std::vector<DirectX::XMFLOAT3>> vertPos, const std::string& key);
#pragma endregion


#pragma region �A�j���[�V����

	/// <summary>
	/// �_���琶�������|���S���̕\���͈͂�ݒ肵�܂��B
	/// </summary>
	/// <param name="leftUpPos">�e�N�X�`���̍�����W</param>
	/// <param name="rightDownPos">�e�N�X�`���̉E�����W</param>
	/// <param name="p">�_</param>
	/// <param name="num">�ԍ�</param>
	//void setPointBoardAnimation
	//(
	//	const DirectX::XMFLOAT2& leftUpPos,
	//	const DirectX::XMFLOAT2& rightDownPos,
	//	const int& p,
	//	const int& num
	//);

	//void setAnimation(int polyNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);
	//void setAnimation2(int polyNum, int dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY);

	void SetSpriteAnimationVertex
	(
		const int& spriteNum, 
		const int& textureNum, 
		const DirectX::XMFLOAT2& maxSize, 
		const DirectX::XMFLOAT2& currentNumber
	);
	void SetSpriteAnimationVertex2(int spriteNum, int textureNum, int posX, int posY, int areaWidth, int areaHeight, int startAreaX, int startAreaY, int endAreaX, int endAreaY);

	void SetSprite3DAnimation
	(
		DirectX::XMFLOAT2 leftUpPosition,
	    DirectX::XMFLOAT2 rightDownPosition,
		const int& spriteNumber,
		const int& textureNumber
	);

#pragma endregion



#pragma region �s��ɂ��ϊ�
	/// <summary>
	/// �x�N�g���̌�������]�s��ŃJ�����̃A���O������]���ĕύX���A�߂��܂�
	/// </summary>
	/// <param name="vector">�x�N�g��</param>
	/// <param name="flag">���K�����邩�ǂ���(true�Ő��K������)</param>
	/// <returns></returns>
	DirectX::XMFLOAT3 MatchEyeVelocityToCamera(DirectX::XMFLOAT3 eyeVector, bool flag);

	/// <summary>
	///	�s��ŉ�]�A�ړ��������J�����̍��W��߂��܂�
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetRotateCameraPosition();


	/// <summary>
	/// �s��ŉ�]�A�ړ��������J�����̒����_��߂��܂�
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 GetRotateCameraTarget();
#pragma endregion

#pragma region �����\��
	void DrawSpriteFontString(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, std::string text, int spriteFontTextureNum);
#pragma endregion

#pragma region �e�q�\��
	//���[�g�I�u�W�F�N�g�̎q�̎q�Ƀ��[�g�I�u�W�F�N�g�Ǝq�̍s�����Z���邽�߂ɁA
	//�e�̃I�u�W�F�N�g���L�^����z����?

	void SetParent
	(
		const std::string& key,
		const int& number,
		const std::string& parentKey,
		const int& parentNum
	);

#pragma endregion

#pragma region �v�Z

	void CalculationNormal
	(
		DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT3 pos3,
		DirectX::XMFLOAT3& normal1, DirectX::XMFLOAT3& normal2, DirectX::XMFLOAT3& normal3
	);
#pragma endregion

#pragma region ���C�u�����g�p�֐�

	void SortModelData(std::vector<std::tuple<ModelData, int>>& modelDatas);
#pragma endregion

};

