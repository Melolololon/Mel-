#pragma once

//モデルデータ
#include"PrimitiveModelData.h"
#include"ObjModelData.h"

//モデルオブジェクト



//あとでモデルクラスに変える
class ModelManager
{
private:
	enum ModelType
	{
		MODEL_PRIMITIVE,
		MODEL_OBJ,
		MODEL_FBX,
	};
	
	static std::unordered_map<std::string, ModelType>modelTypes;
	static std::unordered_map<std::string, std::unique_ptr<PrimitiveModelData>>pPrimitiveModelDatas;
	static std::unordered_map<std::string, std::unique_ptr<ObjModelData>>pObjModelDatas;

	static bool LoadObj(const std::string& path, const std::string& name);
	static bool LoadFbx(const std::string& path, const std::string& name);

public:
	/// <summary>
	/// モデルを読み込みます。
	/// </summary>
	/// <param name="path">モデルのパス</param>
	/// <param name="name">モデル名</param>
	/// <returns></returns>
	static bool Load(const std::string& path,const std::string& name);

	/// <summary>
	/// Load関数で読み込んだモデルをもとにオブジェクトを生成します。
	/// </summary>
	/// <param name="modelName">モデル名</param>
	/// <param name="name">オブジェクト名</param>
	static void CreateLoadModel(const std::string& modelName,const std::string& name );

	/// <summary>
	///  ライブラリに実装されているプリミティブモデルを生成します。
	/// </summary>
	/// <param name="modelType">形状</param>
	/// <param name="name">モデル名</param>
	static void CreatePrimitiveModel(const PrimitiveModelData::PrimityveModelType& modelType,const std::string& name);

};

