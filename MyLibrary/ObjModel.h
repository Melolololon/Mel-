#pragma once
#include"Model.h"

//PrimitibModel���p������ƁA�v���~�e�B�u���f�����쐬����֐���
//���̃N���X��n���Ă��܂����߁AModel�ɕύX

class ObjModel:public Model
{
private:
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

	//���f���t�@�C���ɂ���I�u�W�F�N�g��
	int loadFileObjectNum;
public:
	ObjModel();
	virtual ~ObjModel();

	/// <summary>
	/// obj���f���̒��_��ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path">���f���̃p�X</param>
	/// <param name="loadUV">uv��ǂݍ��ނ�</param>
	/// <param name="loadNormal">normal��ǂݍ��ނ�</param>
	void loadModelVertices
	(
		const std::string& path,
		const bool& loadUV,
		const bool& loadNormal
	);

	void loadModelMaterial
	(
		const int& createNum,
		const size_t& constDataSize = 0
	);
};

