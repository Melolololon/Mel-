#pragma once
#include<vector>
#include"DirectXStruct.h"
#include<unordered_map>
class ModelLoader
{
private:
	ModelLoader();
	~ModelLoader();

public:
	ModelLoader(const ModelLoader& moder) = delete;
	ModelLoader& operator =(const ModelLoader& moder) = delete;
	static ModelLoader* getInstance();

	bool loadPMDModel(const char* path, std::vector<PMDVertex>& vertex, std::vector<unsigned short>& indices);

	/// <summary>
	/// objを読み込みます
	/// </summary>
	/// <param name="path"></param>
	/// <param name="loadUV"></param>
	/// <param name="loadNormal"></param>
	/// <param name="vertex"></param>
	/// <param name="indices"></param>
	/// <param name="materialName">マテリアル名を入れるstring変数のポインタ(マテリアルを読み込まない場合は、nullptrを書く)</param>
	/// <returns></returns>
	bool loadOBJModel
	(
		const char* path,
		bool loadUV,
		bool loadNormal,
		std::vector<std::vector<Vertex>>& vertices,
		std::vector<std::vector<USHORT>>& indices,
		std::string* materialFileName,
		std::vector<std::string>&materialName,
		std::vector<std::unordered_map < USHORT, std::vector<USHORT>>>& smoothNormalCalcData,
		int* loadNum,
		std::vector<DirectX::XMFLOAT3>* bonePosVector,
		std::vector<std::vector<int>>* boneNumVector
	);
	
	bool loadObjMaterial
	(
		std::string materialDirectoryPath, 
		std::string materialFileName, 
		std::vector<Material>& material,
		int* loadCount
	);
};

