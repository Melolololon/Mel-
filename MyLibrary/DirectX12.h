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
#include"DrawGraphicStruct.h"

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


//����DirectXStruct�Ɉڂ��āA���f���̒��_�o�b�t�@���Ƃ���ł���enum�Z�b�g���Ă���������
//����p�C�v���C���Z�b�g����Ƃ��ɐݒ肷��悤�ɂ���?
//���̏ꍇ�A�q�[�v�̍\���̂�������ق�������?
enum VertexType
{
	VERTEX_TYPE_NORMAL,//���W�Auv�A�@���̂�
	VERTEX_TYPE_OBJ_ANIMATION,//���W�Auv�A�@���A�{�[���ԍ�
	VERTEX_TYPE_USER_VERTEX,//���p�҂̎���f�[�^

};
//�e�f�[�^�ɃA�N�Z�X���邽�߂̂���
#pragma region �o�b�t�@�f�[�^�L�[
struct VertexDataKey
{
	std::string key;//�n���h���ɑ���A�N�Z�X���邽�߂̕ϐ��B�o�b�t�@�����O�Ɏ����Őݒ肷��
	VertexType typr;//���_�\���̂̎��
};

struct HeapDataKey
{
	std::string key;
};

struct ObjectData3D
{
	VertexDataKey vertexBufferData;
	HeapDataKey heapDataKey;
};

#pragma endregion



using namespace Microsoft::WRL;
class DirectX12
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
	std::vector<ComPtr<ID3D12PipelineState>> pointPipelineStates;
	std::vector<ComPtr<ID3D12PipelineState>> objPipelineStates;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC spriteGpipeline;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pointGpipeline;
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;
	int startPipelineCreateNum;//Initialize���ɍ��3D�̃p�C�v���C����

	int pipelineNum;
	int spritePipelineNum;


#pragma endregion

#pragma region �o�b�t�@

#pragma region ���_ �C���f�b�N�X
	//[���f������][obj�̒��̃��f������]
	std::unordered_map<std::string,std::vector<VertexBufferSet>> vertexBufferSet;
	std::unordered_map<std::string,std::vector<IndexBufferSet>>indexBufferSet;

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

	CreateBuffer* createBuffer;
	CreatePipeline* createPipeline;
	CreatePolygon* createPolygon;
	CreateCamera* mainCamera;

#pragma endregion


#pragma region ���_

#pragma region 3D
	std::unordered_map<std::string, std::vector<std::vector<Vertex>>> vertices;//[���f������(key�ŃA�N�Z�X)][obj�ɂ��郂�f������][���_����]
	//std::unordered_map<std::string, std::vector<std::vector<OBJAnimationVertex>>> objAnimationVertices;//Vertex�ɉ����A�{�[���ԍ���ǉ�
	std::unordered_map<std::string, std::vector<std::vector<int>>> objAnimationBoneNums;//�{�[���ԍ�


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
		MATERIAL_CONST_BUFFER
	};
	std::unordered_map<std::string, std::vector<ConstBufferTag>>heapTags;

#pragma endregion

#pragma region �e�N�X�`���֌W
	//�摜�f�[�^
	std::unordered_map<std::string, DirectX::TexMetadata> textureData;
	std::vector<DirectX::TexMetadata> spriteTextureData;

#pragma endregion


#pragma endregion



	//�����Ă������
	std::unordered_map<std::string,PolyData> polyDatas;
	std::unordered_map<std::string, HeapData> heapDatas;
	//�擾�����X�v���C�g�̃|�C���^
	std::vector<int*>spriteP;
	std::vector<int*>pointP;

	//���ꂢ��Ȃ�
	std::vector<DirectX::XMFLOAT2>spriteSizes;

#pragma region �J�E���g�ϐ�

#pragma region ���_


	//struct Vertex�Ńo�b�t�@���������
	int createNormalVertexBufferCount;

	//struct OBJAnimationVertex�Ńo�b�t�@���������
	int createOBJAnimationVertexBufferCounter;

#pragma endregion

#pragma region ����ȊO


	int createHeapCount;
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

	bool smoothing;
#pragma endregion


#pragma region �I�u�W�F�N�g���
	struct ObjectConstData
	{
		std::vector<DirectX::XMFLOAT3>position;
		std::vector<DirectX::XMFLOAT3>scale;
		std::vector<DirectX::XMFLOAT3>angle;
		std::vector<DirectX::XMFLOAT4>addColor;
		std::vector<DirectX::XMFLOAT4>subColor;
		std::vector<DirectX::XMFLOAT4>mulColor;
		std::vector<float>pushPolygonNum;
	};
	std::unordered_map < std::string, ObjectConstData>objectConstData;


	std::unordered_map<std::string,std::vector<Material>>materials;
	 

	//std::vector<DirectX::XMVECTOR> spritePosition;
	std::vector<DirectX::XMFLOAT2> spriteScale;
	std::vector<float> spriteAngle;



#pragma endregion

#pragma region �X�v���C�g�t�H���g
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
	void resizeObjectData(int objectNum,const std::string& key);

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



	/// <summary>
	/// ���C�u�������p�C�v���C�����쐬���܂�
	/// </summary>
	void create3DObjectPipeline();
#pragma endregion





public:
	DirectX12(HWND hwnd, int windouWidth, int windowHeight);
	~DirectX12();

#pragma region �������Ȃǂ̕K�{����

	/// <summary>
	/// ������
	/// </summary>
	void initialize();

	/// <summary>
	/// �`��R�}���h�Ȃǂ��Ăяo���O�ɌĂԏ���
	/// </summary>
	void preparationToDraw();

	//�`�揈���B�R�}���h���X�g�̖��߂����s�����肷��
	void draw();

	//�ԍ��擾
	//��������łǂ̃N���G�C�g�ԍ��擾���邩�I�ׂ�悤�ɂ���
	CreateNumberSet getCreateNumber();

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
	void getMatrix(float matrix[4][4],const std::string& key, int number);

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
	void createPolygonData(PolyData polygonData,const std::string& key);
	
	void createHeapData(HeapData despData, bool setConstDataFlag,const std::string& key);

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
		unsigned int vertexDataSize,
		unsigned int vertexSumDataSize,
		std::vector<unsigned short>&index, 
		PolyData polyData, 
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
		PolyData data, 
		const std::string& key
	);

	void loadOBJMaterial
	(
		std::string materialDirectoryPath,
		std::string materialFileName, 
		HeapData heapData, 
		bool setConstDataFlag, 
		const std::string& key

	);

	//���ꂢ��Ȃ�
	void loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, PolyData vertData, HeapData heapData);
#pragma endregion


	void loadSpriteFont(const wchar_t* texturePath, DirectX::XMFLOAT2 lineNum, DirectX::XMFLOAT2 fontSize);

	void loadTexture(const wchar_t* texturePath, Color color);

	//font�́Acreate������̂��X�v���C�g�t�H���g���ǂ���
	void createSprite(int* spr, bool font);


#pragma endregion

#pragma region �`��
	//�`�掞����Ȃ��č��W�Ƃ��Z�b�g�����Ƃ��ɖ���}�b�v����?
//��������Ə������x���Ȃ�?
	void setCmdList(const std::string& vKey, const std::string& hKey, int number);
	void map(const std::string& vKey, const std::string& hKey, int number);

	void spriteSetCmdList(int spriteNum, int textureNum);

	void spriteMap(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, int spriteNum, int textureNum);

	void pointSetCmdList(DirectX::XMFLOAT3 pos, int pointNum, int textureNum, int num);

#pragma endregion

#pragma region �폜
	void deletePolygonData(const std::string key);
	void deleteHeapData(const std::string& key);

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


	void setMulColor(Color color, const std::string& key ,int number);
	void setAddColor(Color color, const std::string& key ,int number);
	void setSubColor(Color color, const std::string& key ,int number);

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
	void setObjectScale(DirectX::XMFLOAT3 angle,  const std::string& key , int number);
	void setObjectAngle(DirectX::XMFLOAT3 angle,  const std::string& key , int number);
	void setObjectPushNum(float objectEX, const std::string& key, int number);

	void spriteSetObjectPosition(DirectX::XMFLOAT2 position, int spriteNum);
	void spriteSetObjectScale(DirectX::XMFLOAT2 scale, int spriteNum);
	void spriteSetObjectAngle(float angle, int spriteNum);

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

#pragma endregion

#pragma region �����W�擾
	std::vector<std::vector<DirectX::XMFLOAT3>> getObjectVertexPosition(const std::string& key);

	bool overrideWriteVertexPosition(std::vector<std::vector<DirectX::XMFLOAT3>> vertPos, const std::string& key);
#pragma endregion


#pragma region �A�j���[�V����

	//void setAnimation(int polyNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);
	//void setAnimation2(int polyNum, int dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY);

	void setSpriteAnimationVertex(int spriteNum, int textureNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY);
	void setSpriteAnimationVertex2(int spriteNum, int textureNum, int posX, int posY, int areaWidth, int areaHeight, int startAreaX, int startAreaY, int endAreaX, int endAreaY);
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
		const std::string& parentKey ,
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


};

