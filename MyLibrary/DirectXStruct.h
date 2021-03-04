#pragma once
#include<vector>
#include<d3d12.h>
#include<DirectXMath.h>
#include<wrl.h>
using namespace Microsoft::WRL;

//DirectX12�ŕ`�悷�邽�߂Ɏg�p����\����
#pragma region �V�F�[�_�[�ɑ�����
//���_�o�b�t�@�ő�����
struct Vertex
{
	DirectX::XMFLOAT3 pos;

	DirectX::XMFLOAT2 uv;//�|���S���̂ǂ���ӂ�������킷���́@�|���S����̍��W
	DirectX::XMFLOAT3 normal;
};

struct SpriteVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};


struct PointVertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT4 color;

};

//�萔�o�b�t�@�ő�����
//�F������薾�邭�������ꍇ�A���X������Ԃ̉摜��p��
//����Ŗʓ|��������add��sub����������
//�����A���Z���Z��Z�̏��ɂ�茋�ʂ��ς�邩�璍��
struct ConstBufferData
{
	DirectX::XMFLOAT4 color;
	DirectX::XMMATRIX mat;
	DirectX::XMMATRIX normalMat;
	DirectX::XMFLOAT4 mulColor;
	DirectX::XMFLOAT4 addColor;
	DirectX::XMFLOAT4 subColor;
	float ex;
	DirectX::XMMATRIX worldMat;
};

struct CommonConstData
{
	//float4�ɂ��邩�p�b�N�l�߂Ȃ��Ƒ���Ȃ�����float4�ɂ��Ă�
	DirectX::XMFLOAT4 lightColor;
	DirectX::XMFLOAT4 light;
	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMMATRIX lightMat;//���C�g�p�s��
};

struct SpriteConstBufferData
{
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT4 addColor;
	DirectX::XMMATRIX mat;
};

struct PointConstBufferData
{
	DirectX::XMMATRIX mat;
	DirectX::XMMATRIX billboardMat;
};

//�e�N�X�`���o�b�t�@�ő�����
struct RGBA
{
	unsigned char r, g, b, a;
};


#pragma endregion


#pragma region ���f����

#pragma region PMD


struct PMDVertex
{
#pragma pack(2)

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	unsigned short bone[2];
	unsigned char boneWeight;
	unsigned char edgeFlg;

#pragma pack()
};

struct PMDHeader
{
	float version;
	char modelName[20];
	char comment[256];
};
#pragma endregion

#pragma region OBJ


struct Material
{
	DirectX::XMFLOAT3 ambient;
	DirectX::XMFLOAT3 diffuse;
	DirectX::XMFLOAT3 specular;
	float alpha;
	std::string materialName;
	std::string textureName;

	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}
};

struct MaterialConstBuffData
{
	DirectX::XMFLOAT3 ambient;
	float pad1;
	DirectX::XMFLOAT3 diffuse;
	float pad2;
	DirectX::XMFLOAT3 specular;
	float alpha;
};
#pragma endregion

#pragma endregion

#pragma region �e�N�X�`����

#pragma region ���͓ǂݍ���

#pragma pack(2)
//BMP��ǂݍ��ݎ��Ɏg�p
struct BMPData
{
	char fileType[2];//�t�@�C���`��
	long length;//�t�@�C���T�C�Y
	short areaOne;//�\���̈�1
	short areaTwo;//�\���̈�2
	unsigned long offset;//�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g

	unsigned long size;//�T�C�Y
	unsigned long width;//��
	unsigned long height;//�c
	short plane;//�v���[����
	short bit;//�F�r�b�g��
	unsigned long compression;//���k�`��
	unsigned long imageSize;//�摜�f�[�^�T�C�Y
	long meterX;//�����𑜓x
	long meterY;//�����𑜓x
	unsigned long parret;//�i�[�p���b�g
	unsigned long important;//�d�v�F�f
	std::vector<RGBA> rgbaData;
};
#pragma pack()


#pragma endregion

#pragma region �X�v���C�g�t�H���g
struct SpriteFontData
{
	int widthLineNum;//����s�̕�����
	int heightLineNum;//����s�̕�����
	int fontSizeX;
	int fontSizeY;
};

#pragma endregion


#pragma endregion

#pragma region �o�b�t�@��

struct WorldMatData 
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 angle;
	DirectX::XMFLOAT3 scale;
};

#pragma region �ėp�o�b�t�@�Z�b�g

//�o�b�t�@�[�AMap���邽�߂̕ϐ��A�r���[�̃Z�b�g
#pragma region ���_

struct VertexBufferSet
{
	ComPtr<ID3D12Resource> vertexBuffer;
	Vertex* vertexMap;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
	std::string materialName;
};

struct PMDVertexBufferSet
{
	ComPtr<ID3D12Resource> vertexBuffer;
	PMDVertex* vertexMap;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
};
#pragma endregion


struct IndexBufferSet
{
	ComPtr<ID3D12Resource> indexBuffer;
	unsigned short* indexMap;
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
};

struct ConstBufferSet
{
	//�������������Ȃ邩�牼�ɔz��g���ĂȂ�

	//obj�̃��f�����ƂɕύX�ł���悤�ɂ��邽�߂ɔz��
	std::vector<ComPtr<ID3D12Resource>> constBuffer;
};

struct TextureBufferSet
{
	//�������������Ȃ邩�牼�ɔz��g���ĂȂ�

	//.obj���̃I�u�W�F�N�g���ƂɃe�N�X�`���Ⴄ�\�����邩��z��
	std::vector<ComPtr<ID3D12Resource>> textureBuff;
};


#pragma endregion

#pragma region ���[�U�[�o�b�t�@
//
////���ꂢ���?
//struct UserVertexBufferSet
//{
//	ComPtr<ID3D12Resource> vertexBuffer;
//	void* vertexMap;
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
//};
//
//
//struct UserConstBufferSet
//{
//	ComPtr<ID3D12Resource> constBuffer;
//	void* constMaps;
//};

#pragma endregion


struct DepthBufferSet
{
	ComPtr<ID3D12Resource> depthBuffer;
};

//
//struct Object3DVertData
//{
//	PolyData polyData;
//	std::vector<Vertex> vertices;
//	std::vector<unsigned short> indices;
//	VertexBufferSet vertSet;
//	IndexBufferSet indexSet;
//};
//
//struct Object3DHeapData 
//{
//	std::vector<ComPtr<ID3D12Resource>> constBuffer;
//	ComPtr<ID3D12Resource> textureBuffer;
//	std::vector<DirectX::XMVECTOR>position;
//	DirectX::XMFLOAT3 angle;
//	DirectX::XMFLOAT3 scale;
//};
//
//
//struct Object2DData
//{
//	VertexBufferSet vertSet;
//	ComPtr<ID3D12Resource> constSet;
//	ComPtr<ID3D12Resource> textureBuffer;
//	DirectX::XMVECTOR position;
//	DirectX::XMFLOAT3 angle;
//	DirectX::XMFLOAT3 scale;
//
//};

#pragma region �|�X�g�G�t�F�N�g
struct PostEffectConstData 
{
	DirectX::XMMATRIX worldMat;
};

#pragma endregion


#pragma endregion


#pragma region �V�X�e���n
struct CameraData
{
	float fovY;
	float nearNumber;
	float farNumber;
	//�����ʒu
	DirectX::XMFLOAT3 eye;
	DirectX::XMFLOAT3 target;
	DirectX::XMFLOAT3 up;
	//���݈ʒu(�s��Ōv�Z������)
	DirectX::XMFLOAT3 nowEye;
	DirectX::XMFLOAT3 nowTarget;
	DirectX::XMFLOAT3 nowUp;
	DirectX::XMFLOAT3 eyeAngle;
	DirectX::XMFLOAT3 targetAngle;
	DirectX::XMFLOAT3 upAngle;
};
#pragma endregion



#pragma region �p�C�v���C���ƃ��[�g�V�O�l�`��
//struct PipelineSet
//{
//	ComPtr<ID3D12RootSignature> rootSignature;
//	ComPtr<ID3D12PipelineState> pipelineState;
//	ComPtr<ID3DBlob> errorBlob;
//};
#pragma endregion


struct CreateNumberSet
{
	int polyNum;
	int despNum;
};

