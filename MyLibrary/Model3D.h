#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include<DirectXMath.h>
#include<vector>
#include"DirectXStruct.h"


struct ModelVerticesData
{
	//[obj�ɂ��郂�f������][���_����]
	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<DirectX::XMFLOAT3>>smoothNormal;
	std::vector<std::vector<USHORT>> indices;

	//[obj�̒��̃��f������]
	std::vector<VertexBufferSet> vertexBufferSet;
	std::vector<IndexBufferSet>indexBufferSet;
};



struct ModelHeapData
{
	std::vector < ModelConstData>modelConstData;
	MaterialConstBuffData material;
	DirectX::TexMetadata textureData;

	std::vector<ConstBufferSet> constBufferSet;
	ComPtr<ID3D12Resource>commonBuffers;
	TextureBufferSet textureBufferSet;


	ComPtr<ID3D12DescriptorHeap> basicHeaps;

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
	std::vector<ConstBufferTag>heapTags;

};





//����Ƀo�b�t�@��������ƁA�I�����Ɋm���Ƀo�b�t�@����ł��Ȃ��B
//�v���O�����I������

//�����ObjModel�ȂǂɌp������
//����������ɂ��Ă��AOBJ�̃{�[���Ƃ������o���Ȃ��B
//�Z�b�g�Ƃ��̊֐��������ɈڐA���邵���Ȃ�
//���ꂩ�A�������`��֐���p��(�I�[�o�[���[�h��)
class Model3D
{
private:
	ModelVerticesData modelVerticesData;
	ModelHeapData modelHeapData;
	

	std::vector <Model3D*> parentModel;
	std::vector <int> parentModelNumbers;




public:
	/*Model3D();
	~Model3D();*/

	void setModelVerticesData(const ModelVerticesData& data)
	{
		modelVerticesData = data;
	}
	void setModelHeapData(const ModelHeapData& data)
	{
		modelHeapData = data;
	}


};

