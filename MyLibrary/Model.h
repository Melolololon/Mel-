#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include"DirectXStruct.h"


#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<dxgi.h>

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
	static ID3D12Device* device;
	static std::vector<ID3D12CommandList*>cmdLists;


	//[obj�̒��̃��f������]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;

	//����X�v���C�g�݂����Ƀq�[�v����Ăяo���Ȃ��悤�ɂ��āA
	//1������������悤�ɂ����ق�������?
	ComPtr<ID3D12Resource>commonBuffers;

	//[�q�[�v�ԍ�]
	std::vector<ConstBufferSet> constBufferSet;
	TextureBufferSet textureBufferSet;
	//�f�B�X�N���v�^�q�[�v�̃o�b�t�@���������邽�߂̂���
	std::vector<ConstBufferTag>heapTags;


	std::vector <Model*> parentModel;
	std::vector <int> parentModelNumbers;


	//�萔�ɃZ�b�g������W�Ȃǂ̒l
	std::vector <ModelConstData>modelConstData;

	std::vector<Material> materials;


#pragma region �o�b�t�@

#pragma region ����


	/// <summary>
	/// �o�b�t�@�̐���
	/// </summary>
	/// <param name="verticesSize"></param>
	/// <param name="verticesNum"></param>
	void createVertexBuffer
	(
		const size_t& verticesSize,
		const size_t& verticesNum
	);

	void createIndexBuffer(const std::vector<USHORT>& indicesNum);

#pragma endregion


#pragma region �}�b�v
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">���f���ԍ�(obj���̂ǂ̃��f����)</param>
	/// <param name="vertexStruct">GPU�̉��z�A�h���X���i�[����|�C���^�̃|�C���^</param>
	void mapVertexBuffer
	(
		const int& modelNum,
		void** vertexStruct
	);

	void unmapVertexBuffer(const int& modelNum);
#pragma endregion


#pragma endregion

#pragma region �X���[�Y�V�F�[�f�B���O
	void calcSmoothingNormal
	(
		std::vector<Vector3> normals
	);
#pragma endregion

public:
	Model();
	virtual ~Model();

	
};

