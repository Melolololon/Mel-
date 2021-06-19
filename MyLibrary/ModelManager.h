#pragma once

//ModelData
#include"PrimitiveModelData.h"
#include"ObjModelData.h"

//ModelObject
#include"ModelObject.h"
#include"ObjModelObject.h"

//シングルトンにする?
class ModelManager
{
private:
	enum ModelType
	{
		MODEL_TYPE_PRIMITIVE,
		MODEL_TYPE_OBJ,
		MODEL_TYPE_FBX,
	};
	static std::unordered_map<std::string, ModelType>modelTypes;

	static std::unordered_map<std::string, std::unique_ptr<PrimitiveModelData>>primitiveModelDatas;
	static std::unordered_map<std::string, std::unique_ptr<ObjModelData>>objModelDatas;

	static std::unordered_map<std::string, std::unique_ptr<ModelObject>>primitiveModelObjects;
	static std::unordered_map<std::string, std::unique_ptr<ObjModelObject>>objModelObjects;

public:

	/// <summary>
	/// モデルの頂点データ、マテリアルを読み込みます。
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	static bool Load(const std::string& path, const std::string& name);

	/// <summary>
	/// モデルの座標などのデータをまとめたものを作成します。この関数で生成したものにモデルを割り当てることで、モデルを描画することができます。
	/// </summary>
	/// <param name="modelName"></param>
	/// <param name="name"></param>
	static bool CreateModelObjectData
	(
		const std::string& modelName,
		const std::string& name,
		ConstBufferData* userConstBufferData = nullptr
	);

	/// <summary>
	/// Load関数で読み込んだモデルを削除します。
	/// </summary>
	/// <param name="name"></param>
	static void DeleteLoadModel(const std::string& name);

	/// <summary>
	/// CreateModelObjectDataで生成したデータを削除します。
	/// </summary>
	/// <param name="name"></param>
	static void DeleteModelObjectData(const std::string& name);
};

