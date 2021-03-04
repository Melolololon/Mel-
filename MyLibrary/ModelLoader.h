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
	/// obj��ǂݍ��݂܂�
	/// </summary>
	/// <param name="path"></param>
	/// <param name="loadUV"></param>
	/// <param name="loadNormal"></param>
	/// <param name="vertex"></param>
	/// <param name="indices"></param>
	/// <param name="materialName">�}�e���A����������string�ϐ��̃|�C���^(�}�e���A����ǂݍ��܂Ȃ��ꍇ�́Anullptr������)</param>
	/// <returns></returns>
	bool loadOBJModel
	(
		const char* path,
		bool loadUV,
		bool loadNormal,
		std::vector<std::vector<Vertex>>& vertices,
		std::vector<std::vector<unsigned short>>& indices,
		std::string* materialFileName,
		std::vector<std::string>&materialName,
		std::vector<std::unordered_map < USHORT, std::vector<USHORT>>>& smoothNormalCalcData,
		int* loadNum
	);
	
	bool loadObjMaterial(std::string materialDirectoryPath, std::string materialFileName, std::vector<Material>& material,int* loadCount);
};

