#pragma once
#include"ModelData.h"

class FbxModelData:public ModelData
{
private:
	std::vector<std::vector<FbxVertex>>vertices;
	//�X���[�Y�V�F�[�f�B���O�p�@��
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;


	//[obj���̃I�u�W�F�N�g��]�X���[�Y�V�F�[�f�B���O�̌v�Z�p�f�[�^
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;



public:

	bool Load(const std::string& path);

	static void Initialize();
};

