#pragma once

//ModelData
#include"PrimitiveModelData.h"
#include"ObjModelData.h"

//ModelObject
#include"ModelObject.h"
#include"ObjModelObject.h"

//�V���O���g���ɂ���?
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
	/// ���f���̒��_�f�[�^�A�}�e���A����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path"></param>
	/// <param name="name"></param>
	static bool Load(const std::string& path, const std::string& name);

	/// <summary>
	/// ���f���̍��W�Ȃǂ̃f�[�^���܂Ƃ߂����̂��쐬���܂��B���̊֐��Ő����������̂Ƀ��f�������蓖�Ă邱�ƂŁA���f����`�悷�邱�Ƃ��ł��܂��B
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
	/// Load�֐��œǂݍ��񂾃��f�����폜���܂��B
	/// </summary>
	/// <param name="name"></param>
	static void DeleteLoadModel(const std::string& name);

	/// <summary>
	/// CreateModelObjectData�Ő��������f�[�^���폜���܂��B
	/// </summary>
	/// <param name="name"></param>
	static void DeleteModelObjectData(const std::string& name);
};

