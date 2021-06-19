#pragma once
#include"ModelData.h"
class ObjModelData :public ModelData
{
private:
	std::vector<std::vector<ObjAnimationVertex>>vertices;
	//�X���[�Y�V�F�[�f�B���O�p�@��
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;


	//[obj���̃I�u�W�F�N�g��]�X���[�Y�V�F�[�f�B���O�̌v�Z�p�f�[�^
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//[.obj���̃I�u�W�F�N�g����][���_����]
	std::vector<std::vector<int>> objBoneNums;//���_�Ɋ��蓖�Ă��Ă���{�[���ԍ�
	//[���f������(key�ŃA�N�Z�X)][�{�[������]
	std::vector<DirectX::XMFLOAT3> objBonePositions;//�{�[�����W


public:

	bool Load(const std::string& path);

	static bool Initialize();
};

