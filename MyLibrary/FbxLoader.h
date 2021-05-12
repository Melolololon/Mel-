#pragma once

#include"FbxModel.h"

#include<string>
#include<d3dx12.h>
#include<fbxsdk.h>

#ifdef _DEBUG
#pragma comment(lib,"libfbxsdk-md.lib")
#pragma comment(lib,"libxml2-md.lib")
#pragma comment(lib,"zlib-md.lib")
#else
#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libxml2-mt.lib")
#pragma comment(lib,"zlib-mt.lib")
#endif // _DEBUG


class FbxLoader final
{
private:
	FbxLoader();
	~FbxLoader();


	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;

	std::string modelDirectryPath;

	/// <summary>
	/// �m�[�h�\����͗p�֐�
	/// </summary>
	/// <param name="model">���f���̃|�C���^</param>
	/// <param name="fbxNode">��͂���m�[�h</param>
	/// <param name="parentNode">�e�m�[�h</param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode,Node* parentNode = nullptr);

	/// <summary>
	/// ���b�V���ǂݍ��݊֐�
	/// </summary>
	/// <param name="">���f��</param>
	/// <param name="node">�m�[�h</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* node);

	/// <summary>
	/// ���_�ǂݍ���
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="fbxMesh">���b�V��</param>
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �ʏ��(�C���f�b�N�X�AUV�A�@��)�ǂݎ��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="fbxMesh">���b�V��</param>
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �}�e���A���ǂݎ��
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="fbxNode">�m�[�h</param>
	void ParseMaterial(FbxModel* fbxModel,FbxNode* fbxNode);
	
	/// <summary>
	/// �X�L�j���O���ǂݎ��
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void ParseSkin(FbxModel* model, FbxMesh* fbxMesh);


	/// <summary>
	/// Fbx�̍s�񂩂�XMMATRIX�ɕϊ�����N���X
	/// </summary>
	/// <param name="dst"></param>
	/// <param name="src"></param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);


	std::string ExtractFileName
	(
		const std::string& path, 
		std::string* directry, 
		std::string* fileName
	);

public:
	FbxLoader(const FbxLoader& model) = delete;
	FbxLoader& operator =(const FbxLoader& model) = delete;
	static FbxLoader* GetInstance();

	/// <summary>
	/// ���������܂�
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void Initialize(ID3D12Device* device);
	
	void Finalize();

	/// <summary>
	/// fbx��ǂݍ��݂܂�
	/// </summary>
	/// <param name="modelPath">���f���̃p�X</param>
	/// <param name="fbxModel">���f���̃|�C���^</param>
	void LoadFbxModel(const std::string& modelPath,FbxModel* fbxModel);
};

