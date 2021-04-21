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
class Model
{
public:

	//�萔�o�b�t�@�̃^�O(���[�g�p�����[�^�[�Z�b�g�p)
	//"Texture" �V�F�[�_�[���\�[�X�r���[ t0
	//"LibraryConst" ���C�u�����\���� b0
	//"UserConst" ���[�U�[�\���� b1
	//"Material" �}�e���A���\���� b2
	//COMMON_CONST_BUFFER ���� b3
	enum ConstBufferTag
	{
		TEXTURE_BUFFER,
		COMMON_CONST_BUFFER,
		LIBRARY_CONST_BUFFER,
		USER_CONST_BUFFER,
		MATERIAL_CONST_BUFFER,
		OBJ_BONE_MATRIX_CONST_BUFFER
	};

protected:
	
	////[obj�ɂ��郂�f������][���_����]
	//std::vector<std::vector<Vertex>> vertices;
	//std::vector<std::vector<USHORT>> indices;


	//std::vector < ModelConstData>modelConstData;
	//MaterialConstBuffData material;

	//std::vector <Model3D*> parentModel;
	//std::vector <int> parentModelNumbers;

	//�f�B�X�N���v�^�q�[�v�̃o�b�t�@���������邽�߂̂���
	std::vector<ConstBufferTag>heapTags;

	//[obj�̒��̃��f������]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;

	//����X�v���C�g�݂����Ƀq�[�v����Ăяo���Ȃ��悤�ɂ��āA
	//1������������悤�ɂ����ق�������?
	ComPtr<ID3D12Resource>commonBuffers;

	//[�q�[�v�ԍ�]
	std::vector<ConstBufferSet> constBufferSet;
	TextureBufferSet textureBufferSet;

	std::vector <Model*> parentModel;
	std::vector <int> parentModelNumbers;


	//�萔�ɃZ�b�g�������
	std::vector <ModelConstData>modelConstData;

	std::vector<Material> materials;
public:
	Model();
	virtual ~Model();
};

