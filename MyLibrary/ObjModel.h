#pragma once
#include"Model.h"
#include"Texture.h"
//PrimitibModel���p������ƁA�v���~�e�B�u���f�����쐬����֐���
//���̃N���X��n���Ă��܂����߁AModel�ɕύX

class ObjModel:public Model
{
private:

	static PipelineState defaultObjPipeline;

	std::string materialFileName;

	//���_
	std::vector<std::vector<ObjAnimationVertex>> vertices;
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

	//[obj���̃I�u�W�F�N�g��]�X���[�Y�V�F�[�f�B���O�̌v�Z�p�f�[�^
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//�{�[���̍��W�Ƃ�
	BoneData boneConstData;
	//[.obj���̃I�u�W�F�N�g����][���_����]
	std::vector<std::vector<int>> objBoneNums;//���_�Ɋ��蓖�Ă��Ă���{�[���ԍ�
	//[���f������(key�ŃA�N�Z�X)][�{�[������]
	 std::vector<DirectX::XMFLOAT3> objBonePositions;//�{�[�����W
	//[�{�[������](�I�u�W�F�N�g���Ƃɕς���K�v�Ȃ������Ȃ̂ŁA�������Ă�)
	std::vector<ParentBoneData> parentBoneData;//�e�{�[���Ɖe���x


	std::vector<std::unique_ptr<Texture>>textures;

	/// <summary>
/// obj���f���̒��_��ǂݍ��݂܂��B
/// </summary>
/// <param name="path">���f���̃p�X</param>
/// <param name="loadUV">uv��ǂݍ��ނ�</param>
/// <param name="loadNormal">normal��ǂݍ��ނ�</param>
	void loadModelVertices
	(
		const std::string& path,
		const bool loadUV,
		const bool loadNormal
	);

	void loadModelMaterial
	(
		const std::string& directryPath,
		const int createNum,
		const size_t constDataSize = 0
	);
public:
	ObjModel();
	virtual ~ObjModel();


	/// <summary>
	/// obj���f����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path">�p�X</param>
	/// <param name="loadUV">UV��ǂݍ��ނ��ǂ���</param>
	/// <param name="createNum">������</param>
	/// <param name="constDataSize">�V�F�[�_�[�ɑ��闘�p�҂��p�ӂ����f�[�^�̃o�C�g��</param>
	void loadModel
	(
		const std::string& path,
		const bool loadUV,
		const int createNum,
		const size_t constDataSize = 0
	);


	static bool initialize();


	static void mapBoneData();
};

