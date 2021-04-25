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
	void parseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode,Node* parentNode = nullptr);

#pragma region ���b�V���ǂݍ���


	/// <summary>
	/// ���b�V���ǂݍ��݊֐�
	/// </summary>
	/// <param name="">���f��</param>
	/// <param name="node">�m�[�h</param>
	void parseMesh(FbxModel* fbxModel, FbxNode* node);

	/// <summary>
	/// ���_�ǂݍ���
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void parseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �ʏ��(�C���f�b�N�X�AUV�A�@��)�ǂݎ��
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void parseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);

	/// <summary>
	/// �}�e���A���ǂݎ��
	/// </summary>
	/// <param name="model"></param>
	/// <param name="fbxMesh"></param>
	void parseMaterial(FbxModel* fbxModel,FbxNode* fbxNode);
	
	std::string extractFileName
	(
		const std::string& path, 
		std::string* directry, 
		std::string* fileName
	);

#pragma endregion
public:
	FbxLoader(const FbxLoader& moder) = delete;
	FbxLoader& operator =(const FbxLoader& moder) = delete;
	static FbxLoader* getInstance();

	/// <summary>
	/// ���������܂�
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	void initialize(ID3D12Device* device);
	
	void end();

	/// <summary>
	/// fbx��ǂݍ��݂܂�
	/// </summary>
	/// <param name="modelPath">���f���̃p�X</param>
	void loadFbxModel(const std::string& modelPath,FbxModel* fbxModel);
};

