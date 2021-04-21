#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include<DirectXMath.h>
#include<vector>
#include"DirectXStruct.h"

//�Ƃ肠��������̃|�C���^���L�[�ɂ���

//����Ƀo�b�t�@��������ƁA�I�����Ɋm���Ƀo�b�t�@����ł��Ȃ��B
//�v���O�����I������

//�����ObjModel�ȂǂɌp������
//����������ɂ��Ă��AOBJ�̃{�[���Ƃ������o���Ȃ��B
//�Z�b�g�Ƃ��̊֐��������ɈڐA���邵���Ȃ�
class Model3D
{
private:
	//[obj�ɂ��郂�f������][���_����]
	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<USHORT>> indices;


	std::vector < ModelConstData>modelConstData;
	MaterialConstBuffData material;

	std::vector <Model3D* > parentModel;
	std::vector <int> parentModelNumbers;
public:
	Model3D();
	~Model3D();
};

