#pragma once
#include"Model.h"

//PrimitibModel���p������ƁA�v���~�e�B�u���f�����쐬����֐���
//���̃N���X��n���Ă��܂����߁AModel�ɕύX

class ObjModel:public Model
{
private:


	//���_
	std::vector<std::vector<Vertex>> vertices;
	//�C���f�b�N�X
	std::vector<std::vector<USHORT>> indices;

	BoneData boneConstData;


	//[.obj���̃I�u�W�F�N�g����][���_����]
	std::vector<std::vector<int>> objBoneNums;//���_�Ɋ��蓖�Ă��Ă���{�[���ԍ�
	//[���f������(key�ŃA�N�Z�X)][�{�[������]
	 std::vector<DirectX::XMFLOAT3> objBonePositions;//�{�[�����W


	//[�{�[������](�I�u�W�F�N�g���Ƃɕς���K�v�Ȃ������Ȃ̂ŁA�������Ă�)
	std::vector<ParentBoneData> parentBoneData;//�e�{�[���Ɖe���x


public:
	ObjModel();
	~ObjModel();
};

