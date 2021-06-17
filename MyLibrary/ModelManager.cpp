#include "ModelManager.h"

std::unordered_map<std::string, ModelManager::ModelType>ModelManager::modelTypes;
std::unordered_map<std::string, std::unique_ptr<PrimitiveModelData>>ModelManager::pPrimitiveModelDatas;
std::unordered_map<std::string, std::unique_ptr<ObjModelData>>ModelManager::pObjModelDatas;

bool ModelManager::LoadObj(const std::string& path, const std::string& name)
{
	ObjModelData* pModelData = nullptr;
	pObjModelDatas.emplace(name, std::make_unique<ObjModelData>());
	pModelData = pObjModelDatas[name].get();

	return pModelData->Load(path);
}

bool ModelManager::LoadFbx(const std::string& path, const std::string& name)
{
	return false;
}

bool ModelManager::Load(const std::string& path, const std::string& name)
{
	if(path.find(".obj") != std::string::npos)
	{
		modelTypes.emplace(name, MODEL_OBJ);
		return LoadObj(path,name);
	}
	else if(path.find(".fbx") != std::string::npos)
	{
		modelTypes.emplace(name, MODEL_FBX);
	}
	else
	{
#ifdef _DEBUG
		OutputDebugString(L"ModelManager::Load()_Ç±ÇÃÉÇÉfÉãÇÕì«Ç›çûÇﬂÇ‹ÇπÇÒÅB\n");
#endif // _DEBUG

		return false;

	}


}

void ModelManager::CreateLoadModel(const std::string& modelName, const std::string& name)
{
}

void ModelManager::CreatePrimitiveModel(const PrimitiveModelData::PrimityveModelType& modelType, const std::string& name)
{

}
