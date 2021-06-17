#pragma once

//���f���f�[�^
#include"PrimitiveModelData.h"
#include"ObjModelData.h"

//���f���I�u�W�F�N�g



//���ƂŃ��f���N���X�ɕς���
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
	/// ���f����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path">���f���̃p�X</param>
	/// <param name="name">���f����</param>
	/// <returns></returns>
	static bool Load(const std::string& path,const std::string& name);

	/// <summary>
	/// Load�֐��œǂݍ��񂾃��f�������ƂɃI�u�W�F�N�g�𐶐����܂��B
	/// </summary>
	/// <param name="modelName">���f����</param>
	/// <param name="name">�I�u�W�F�N�g��</param>
	static void CreateLoadModel(const std::string& modelName,const std::string& name );

	/// <summary>
	///  ���C�u�����Ɏ�������Ă���v���~�e�B�u���f���𐶐����܂��B
	/// </summary>
	/// <param name="modelType">�`��</param>
	/// <param name="name">���f����</param>
	static void CreatePrimitiveModel(const PrimitiveModelData::PrimityveModelType& modelType,const std::string& name);

};

