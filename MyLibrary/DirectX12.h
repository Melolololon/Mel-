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
	CreatePipeline* createPipeline;
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

	//[obj���̃I�u�W�F�N�g��]
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//�X���[�X�V�F�[�f�B���O�p�֐�
	/// <summary>
	/// �X���[�X�V�F�[�f�B���O���s�����߂ɖ@�����v�Z���܂�
	/// </summary>
	void calcSmoothingNormals(const std::string key);


#pragma endregion


#pragma region �I�u�W�F�N�g���
	struct ModelConstData
	{
		std::vector<DirectX::XMFLOAT3>position;
		std::vector<DirectX::XMFLOAT3>scale;
		std::vector<DirectX::XMFLOAT3>angle;
		std::vector<DirectX::XMFLOAT4>addColor;
		std::vector<DirectX::XMFLOAT4>subColor;
		std::vector<DirectX::XMFLOAT4>mulColor;
		std::vector<float>pushPolygonNum;

	};
	std::unordered_map < std::string, ModelConstData>modelConstData;

#pragma region �{�[��

	//�\�����Ă郂�f��1��1���Ⴄ�l�̍\����
	struct BoneData
	{
		//0,0,0����A�ǂ̂��炢����������\����float3
		std::vector<std::vector<DirectX::XMFLOAT3>>moveVector;

		std::vector<std::vector<DirectX::XMFLOAT3>>scale;
		std::vector<std::vector<DirectX::XMFLOAT3>>angle;
	};
	//[key ���f������][�I�u�W�F�N�g�̔ԍ�����][�{�[������]
	std::unordered_map < std::string, BoneData> boneConstData;


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

	//�{�[���̐e���Z�b�g����Ƃ��ɃZ�b�g����l
	struct ParentBoneData
	{
		int parentBoneNum;//-1�𖢃Z�b�g�ɂ��邽�߁Aint�^
		DirectX::XMFLOAT3 angleImpact;
		DirectX::XMFLOAT3 scaleImpact;
		DirectX::XMFLOAT3 moveVectorImpact;
	};
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
	ConstBufferData* constData3D;
	//����
	CommonConstData* commonConstData3D;
	//���[�U�[
	void** userConstData3D;
	unsigned int userConstDataSize3D;
	//�}�e���A���p
	MaterialConstBuffData* materialData;

	//���Ȃ���
	std::vector<ConstBufferData*>spriteConstBufferDatas;

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
	void resizeObjectData(int objectNum, const std::string& key);

	void createHeap();

#pragma region �萔�o�b�t�@

	//���ʃo�b�t�@�쐬������createConstBuffer�݂�����
	//�D���ȃq�[�v�ɍ쐬�ł���悤�ɂ���

	/// <summary>
	/// ���ʃo�b�t�@�쐬
	/// </summary>
	/// <param name="texNum">�e�N�X�`���o�b�t�@��</param>
	/// <param name="key"></param>
	void createCommonBuffer
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
	bool createConstBuffer
	(
		ComPtr<ID3D12DescriptorHeap>& heap,
		const int& heapHandleNum,
		ConstBufferSet* cBufferSet,
		void** constData,
		const UINT& constDataSize
	);

#pragma endregion

#pragma region map����
	void calcBillboardMat(DirectX::XMMATRIX& matWorld);
#pragma endregion


	/// <summary>
	/// ���C�u�������p�C�v���C�����쐬���܂�
	/// </summary>
	void create3DObjectPipeline();
#pragma endregion




	DirectX12();
	~DirectX12();

public:

	DirectX12(DirectX12& d) = delete;
	DirectX12 operator=(DirectX12& d) = delete;
	static DirectX12* getInstance();

#pragma region �������Ȃǂ̕K�{����

	/// <summary>
	/// ������
	/// </summary>
	void initialize(HWND hwnd, int windouWidth, int windowHeight);

	/// <summary>
	/// �`��R�}���h�Ȃǂ��Ăяo���O�ɌĂԏ���
	/// </summary>
	void preparationToDraw();

	//�`�揈���B�R�}���h���X�g�̖��߂����s�����肷��
	void draw();

	void end();
#pragma endregion


	//��ʂ̐F�󂯎��
	void setScreenColor(Color screenColor);

	//�X�v���C�g�̃T�C�Y�擾
	DirectX::XMFLOAT2 getTextureSize(int textureHandle);


	//���ꂢ���
#pragma region �p�C�v���C���̐ݒ�

	void setDespTestFlag(bool flag);

#pragma endregion

#pragma region ���[�U�[�p�C�v���C���֌W

	int getStartPipelineCreateNum();

	/// <summary>
	/// ����V�F�[�_�[���g���ăp�C�v���C�������܂�
	/// </summary>
	/// <param name="vShaderData"></param>
	/// <param name="gSyaderData"></param>
	/// <param name="pShaderData"></param>
	bool createUserPipelineState(
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
	void getMatrix(float matrix[4][4], const std::string& key, int number);

	//���[���h�͏�Z���Ȃ�
	void getCameraMatrix(float matrix[4][4]);

	/// <summary>
/// �C���v�b�g���C�A�E�g�̐ݒ�����܂�
/// </summary>
/// <param name="semantics">�Z�}���e�B�N�X��</param>
/// <param name="num">�f�[�^��(Vector3�̏ꍇ�A�O�����Ȃ̂�3�����)</param>
	void setInputLayout(const std::vector<InputLayoutData>& inputLayoutData);

	/// <summary>
/// ���݂̃C���v�b�g���C�A�E�g���폜���܂�
/// </summary>
	void deleteInputLayout();


#pragma region �|�X�g�G�t�F�N�g
	void setPostEffectPipeline(const int& num);

	bool createUserPostEffectPipelineState(const ShaderData& pShaderData);
#pragma endregion


#pragma endregion

#pragma region �o�b�t�@�쐬
	void createPoint(int createNum, int* point);

	//type�ɉ����Ăǂ�count��++���邩���߂�
	void createPolygonData
	(
		const std::vector<Vertex>& vertex, 
		const std::vector<USHORT>& index,
		const std::string& key
	);

	void createHeapData
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
	void addUserVertex(std::vector<Vector3>& vertexPos, std::vector<Vector2>& vertexUV, const std::string& key);


	void addUserIndex(std::vector<unsigned short>& index, const std::string& key);

	/// <summary>
	/// 
	/// ����̃f�[�^���g�p���A�����Œ��_����ݒ肵�ăI�u�W�F�N�g�𐶐����܂�
	/// </summary>
	/// <param name="vertexData"></param>
	/// <param name="vertexDataSize"></param>
	/// <param name="index"></param>
	/// <param name="polyData"></param>
	void createUserPolygon
	(
		void** vertexData,
		UINT vertexDataSize,
		UINT vertexSumDataSize,
		std::vector<USHORT>&index,
		const std::string& key
	);
#pragma endregion

#pragma region ���f��
	VertexType loadOBJVertex
	(
		const char* path,
		bool loadUV,
		bool loadNormal,
		std::string* materialFireName,
		const std::string& key
	);

	void loadOBJMaterial
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


	void loadSpriteFont(const wchar_t *const texturePath,const DirectX::XMFLOAT2& lineNum);

	void loadTexture(const wchar_t* texturePath, Color color);

	//font�́Acreate������̂��X�v���C�g�t�H���g���ǂ���
	void createSprite(int* spr, bool font);


#pragma endregion

#pragma region �`��
	//�`�掞����Ȃ��č��W�Ƃ��Z�b�g�����Ƃ��ɖ���}�b�v����?
//��������Ə������x���Ȃ�?
	void setCmdList(const ModelData& modelData, int number);
	void map(const ModelData& modelData, int number);

	void spriteSetCmdList(int spriteNum, int textureNum,const bool& sprite3DFlag);

	void spriteMap(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, int spriteNum, int textureNum);
	void spriteMap3D
	(
		const DirectX::XMFLOAT3& position,
		const DirectX::XMFLOAT2& size ,
		const int& spriteNumber,
		const int& textureNumber
	);

	void pointSetCmdList
	(
		DirectX::XMFLOAT3 pos, 
		int pointNum, 
		int textureNum, 
		int num
	);

#pragma endregion

#pragma region �폜
	void deletePolygonData(const ModelData& m);
	void deleteHeapData(const ModelData& m);

	void deleteSprite(int sprite);
#pragma endregion

#pragma region �J����
	void setCameraDataMatrixPoint(Vector3 eye, Vector3 target, Vector3 up);
	void setCameraData(Vector3 eye, Vector3 target, Vector3 up);

	void setCameraVelocity(DirectX::XMFLOAT3 eyeVelocity, DirectX::XMFLOAT3 targetVelocity);

	void setCameraAngre(DirectX::XMFLOAT3 eyeAngle, DirectX::XMFLOAT3 targetAngle, DirectX::XMFLOAT3 upAngle);


	void setNearAndFar(float nearNum, float farNum);
#pragma endregion

#pragma region �����ڑ���

#pragma region �X���[�X�V�F�[�f�B���O
	void setSmoothingFlag(bool flag);
#pragma endregion


	void setMulColor(Color color, const std::string& key, int number);
	void setAddColor(Color color, const std::string& key, int number);
	void setSubColor(Color color, const std::string& key, int number);

	void setIsPlane(bool flag);
	void setIsBillboard(bool x, bool y, bool z);

	void setSpriteMulColor(Color color, int spriteNum);
	void setSpriteAddColor(Color color, int spriteNum);
	void setSpriteSubColor(Color color, int spriteNum);



	void setPointMulColor(Color color, int pointNum, int num);



#pragma endregion

#pragma region ���C�g����
	void setLightVector(DirectX::XMFLOAT3 vector);

	void setLightColor(Color lightColor);
#pragma endregion

#pragma region ����֐�
	void setObjectPosition(DirectX::XMFLOAT3 position, const std::string& key, int number);
	void setObjectScale(DirectX::XMFLOAT3 angle, const std::string& key, int number);
	void setObjectAngle(DirectX::XMFLOAT3 angle, const std::string& key, int number);
	void setObjectPushNum(float objectEX, const std::string& key, int number);

	void spriteSetObjectPosition(DirectX::XMFLOAT2 position, int spriteNum);
	void spriteSetObjectScale(DirectX::XMFLOAT2 scale, int spriteNum);
	void spriteSetObjectAngle(const DirectX::XMFLOAT3& angle,const int& spriteNum);

	void changeSpriteSize(DirectX::XMFLOAT2 size, int spriteData);


	void setPointScale(DirectX::XMFLOAT2 scale, int pointNum, int num);



#pragma region �|�X�g�G�t�F�N�g

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̍��W��ύX���܂�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rtNum">�ǂ̃����_�[�^�[�Q�b�g���w�肷�邩(���͈Ӗ��Ȃ�)</param>
	void setRenderTargerPosition(const DirectX::XMFLOAT3& pos, const int& rtNum);

	void setRenderTargetAngle(const DirectX::XMFLOAT3& angle, const int& rtNum);

	void setRenderTargetScale(const DirectX::XMFLOAT3& scale, const int& rtNum);

	void setPostEffectCameraFlag(const bool& flag, const int& rtNum);
#pragma endregion

#pragma region �A�j���[�V����

	std::vector<DirectX::XMFLOAT3> getBonePosition(const std::string& key);

	void setOBJModelRotatePoint
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
	void setOBJBoneMoveVector
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
	void setOBJBoneScale
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
	void setOBJBoneAngle
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
	void setParentOBJBone
	(
		const UINT& boneNum,
		const UINT& parentBoneNum,
		const std::string& key
	);

	void setParentOBJBoneScaleImpact
	(
		const UINT& boneNum,
		const DirectX::XMFLOAT3& scaleImpact,
		const std::string& key
	);
	void setParentOBJBoneAngleImpact
	(
		const UINT& boneNum,
		const DirectX::XMFLOAT3& angleImpact,
		const std::string& key
	);
	void setParentOBJBoneMoveVectorImpact
	(
		const UINT& boneNum,
		const DirectX::XMFLOAT3& moveVectorImpact,
		const std::string& key
	);

#pragma endregion


#pragma endregion

#pragma region �����W�擾
	std::vector<std::vector<DirectX::XMFLOAT3>> getObjectVertexPosition(const std::string& key);

	bool overrideWriteVertexPosition(std::vector<std::vector<DirectX::XMFLOAT3>> vertPos, const std::string& key);
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

	void setSpriteAnimationVertex
	(
		const int& spriteNum, 
		const int& textureNum, 
		const DirectX::XMFLOAT2& maxSize, 
		const DirectX::XMFLOAT2& currentNumber
	);
	void setSpriteAnimationVertex2(int spriteNum, int textureNum, int posX, int posY, int areaWidth, int areaHeight, int startAreaX, int startAreaY, int endAreaX, int endAreaY);

	void setSprite3DAnimation
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
	DirectX::XMFLOAT3 matchEyeVelocityToCamera(DirectX::XMFLOAT3 eyeVector, bool flag);

	/// <summary>
	///	�s��ŉ�]�A�ړ��������J�����̍��W��߂��܂�
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 getRotateCameraPosition();


	/// <summary>
	/// �s��ŉ�]�A�ړ��������J�����̒����_��߂��܂�
	/// </summary>
	/// <returns></returns>
	DirectX::XMFLOAT3 getRotateCameraTarget();
#pragma endregion

#pragma region �����\��
	void drawSpriteFontString(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, std::string text, int spriteFontTextureNum);
#pragma endregion

#pragma region �e�q�\��
	//���[�g�I�u�W�F�N�g�̎q�̎q�Ƀ��[�g�I�u�W�F�N�g�Ǝq�̍s�����Z���邽�߂ɁA
	//�e�̃I�u�W�F�N�g���L�^����z����?

	void setParent
	(
		const std::string& key,
		const int& number,
		const std::string& parentKey,
		const int& parentNum
	);

#pragma endregion

#pragma region �v�Z

	void calculationNormal
	(
		DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT3 pos3,
		DirectX::XMFLOAT3& normal1, DirectX::XMFLOAT3& normal2, DirectX::XMFLOAT3& normal3
	);
#pragma endregion

#pragma region ���C�u�����g�p�֐�

	void sortModelData(std::vector<std::tuple<ModelData, int>>& modelDatas);
#pragma endregion

};

