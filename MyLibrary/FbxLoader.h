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


class FbxLoader
{
private:
	FbxLoader();
	~FbxLoader();


	ID3D12Device* device = nullptr;

	FbxManager* fbxManager = nullptr;

	FbxImporter* fbxImporter = nullptr;

	/// <summary>
	/// �m�[�h�\����͗p�֐�
	/// </summary>
	/// <param name="model">���f���̃|�C���^</param>
	/// <param name="fbxNode">��͂���m�[�h</param>
	void parseNodeRecursive(FbxModel* model, FbxNode* fbxNode);

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
	void loadFbxModel(const std::string& modelPath);
};

