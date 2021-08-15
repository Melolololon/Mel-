#pragma once
#include<vector>
#include"DirectXStruct.h"
#include<unordered_map>
#include"Vector.h"
#include"Material.h"
#include"Values.h"

namespace MelLib
{

	class ModelLoader
	{
	private:
		ModelLoader();
		~ModelLoader();

	public:
		ModelLoader(const ModelLoader& model) = delete;
		ModelLoader& operator =(const ModelLoader& model) = delete;
		static ModelLoader* GetInstance();


		bool LoadPmdModel(const char* path, std::vector<PMDVertex>& vertex, std::vector<unsigned short>& indices);

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
		bool LoadObjModel
		(
			const std::string& path,
			bool loadUV,
			bool loadNormal,
			std::vector<std::vector<FbxVertex>>& vertices,
			std::vector<std::vector<USHORT>>& indices,
			std::string& materialFileName,
			std::vector<std::string>& materialName,
			std::vector<std::unordered_map < USHORT, std::vector<USHORT>>>& smoothNormalCalcData,
			int* loadNum,
			std::vector<Vector3>* bonePosVector,
			std::vector<std::vector<int>>* boneNumVector
		);

		bool LoadObjMaterial
		(
			std::string materialDirectoryPath,
			std::string materialFileName,
			std::vector<std::string>& texPath,
			std::vector<ADSAMaterialData>& materials,
			int* loadCount
		);
	};

}