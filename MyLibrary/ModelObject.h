#pragma once
#include"Model.h"

class ModelObject
{
private:
	//���f��
	Model* model;

	//�萔�o�b�t�@
	std::vector<std::vector<ComPtr<ID3D12Resource>>> constBuffer;//���C��(��{�I�ȏ��)
	std::vector<std::vector<ComPtr<ID3D12Resource>>> materialConstBuffer;//�}�e���A��
	std::vector<std::vector<ComPtr<ID3D12Resource>>> userConstBuffer;//���[�U�[
	std::vector<std::vector<ComPtr<ID3D12Resource>>> modelConstBuffer;//���f�����L

	//�萔�ɃZ�b�g������W�Ȃǂ̒l
	//[���f������][���f�����̃I�u�W�F�N�g��]
	std::vector <std::vector<ModelConstData>>modelConstDatas;
	//[���f�����̃I�u�W�F�N�g����]
	std::vector<Material> materials;

public:

};

