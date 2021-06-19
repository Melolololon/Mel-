#include "ModelManager.h"

std::unordered_map<std::string, ModelManager::ModelType>ModelManager::modelTypes;

std::unordered_map<std::string, std::unique_ptr<PrimitiveModelData>>ModelManager::primitiveModelDatas;
std::unordered_map<std::string, std::unique_ptr<ObjModelData>>ModelManager::objModelDatas;

std::unordered_map<std::string, std::unique_ptr<ModelObject>>ModelManager::primitiveModelObjects;
std::unordered_map<std::string, std::unique_ptr<ObjModelObject>>ModelManager::objModelObjects;

bool ModelManager::Load(const std::string& path, const std::string& name)
{

	if (path.find(".obj") != std::string::npos)
	{
		modelTypes.emplace(name, ModelType::MODEL_TYPE_OBJ);
		objModelDatas.emplace(name, std::make_unique<ObjModelData>());
		ObjModelData* pModelData = objModelDatas[name].get();
		return pModelData->Load(path);
		
	}
	else if (path.find(".fbx") != std::string::npos)
	{
		modelTypes.emplace(name, ModelType::MODEL_TYPE_FBX);
	}
	else
	{
		OutputDebugStringA(path.data());
		OutputDebugStringW(L"を読み込めませんでした。フォーマットが対応していない。または、pathの入力ミスの可能性が考えられます。\n");
	}


}

bool ModelManager::CreateModelObjectData
(
	const std::string& modelName, 
	const std::string& name, 
	ConstBufferData* userConstBufferData
)
{

	if (modelTypes.contains(modelName))
	{
		switch (modelTypes[modelName])
		{
		case ModelManager::MODEL_TYPE_PRIMITIVE:
			primitiveModelObjects.emplace(name, std::make_unique<ModelObject>(objModelDatas[name].get()));
			break;
		case ModelManager::MODEL_TYPE_OBJ:
			objModelObjects.emplace(name, std::make_unique<ObjModelObject>(objModelDatas[name].get()));

			break;
		case ModelManager::MODEL_TYPE_FBX:
			break;
		default:
			break;
		}

		return true;
	}


	OutputDebugStringW(L"ModelData ");
	OutputDebugStringA(modelName.data());
	OutputDebugStringW(L"が見つかりませんでした。ModelObjectDataの生成に失敗しました。\n");
	return false;
	

}

void ModelManager::DeleteLoadModel(const std::string& name)
{

	switch (modelTypes[name])
	{
	case ModelManager::MODEL_TYPE_PRIMITIVE:
		primitiveModelDatas.erase(name);
		break;
	case ModelManager::MODEL_TYPE_OBJ:
		objModelDatas.erase(name);

		break;
	case ModelManager::MODEL_TYPE_FBX:
		break;
	}
}

void ModelManager::DeleteModelObjectData(const std::string& name)
{
	switch (modelTypes[name])
	{
	case ModelManager::MODEL_TYPE_PRIMITIVE:
		primitiveModelObjects.erase(name);
		break;
	case ModelManager::MODEL_TYPE_OBJ:
		objModelObjects.erase(name);

		break;
	case ModelManager::MODEL_TYPE_FBX:
		break;
	}
}
