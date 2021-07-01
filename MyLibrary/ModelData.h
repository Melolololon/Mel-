#pragma once

#include<DirectXMath.h>
#include<vector>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi.h>

#include"DirectXStruct.h"
#include"Texture.h"
#include"Color.h"
#include"BufferData.h"

//obj��fbx�܂Ƃ߂�?
//�������ق��������@�v���~�e�B�u�Ƃ����邵
//�����ƌĂяo�����������N���X�̊֐��g��?

//���f���̒��_�A�C���f�b�N�X�A�e�N�X�`�����܂Ƃ߂��N���X
class ModelData
{
private:
	static ID3D12Device* device;

	//���_
	std::vector<VertexBufferSet> vertexBufferSet;

	//�C���f�b�N�X
	std::vector <IndexBufferSet> indexBufferSet;

	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;

	//�ǂݎ�����}�e���A��
	Material material;

	//�e�N�X�`�����g��Ȃ��ꍇ�̐F
	Color color;


#pragma region ���_

	/// <summary>
	/// ���_�o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="vertexSize"></param>
	/// <param name="vertices"></param>
	void CreateVertexBufferSet
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum
	);

	void MapVertices();

#pragma endregion

#pragma region �C���f�b�N�X


	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBufferSet
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	void MapIndices(const std::vector<std::vector<USHORT>>& indices);
#pragma endregion

#pragma region �e�N�X�`��

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̐���
	/// </summary>
	/// <param name="textureNum"></param>
	void CreateDescriptorHeap(const UINT textureNum);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	void CteateTextureBufferAndViewSetTexture();

	/// <summary>
	/// �e�N�X�`���o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="color"></param>
	void CteateTextureBufferAndViewSetColor();

#pragma endregion



	//���f���t�@�C���ɉ����f�������邩
	int modelFileObjectNum = 0;

	std::vector<std::vector<USHORT>> indices;

	std::vector<std::unique_ptr<Texture>>pTextures;

	/// <summary>
	/// ���_�C���f�b�N�X�o�b�t�@�Z�b�g�̐�����Map���s���܂��B
	/// </summary>
	/// <typeparam name="VERTEX">���_</typeparam>
	/// <param name="vertexSize"></param>
	/// <param name="vertexNum"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	void BufferPreparationSetTexture
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<USHORT>>& indices
	);

	void BufferPreparationSetColor
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<USHORT>>& indices
	);


public:

	ModelData() {}
	~ModelData() {}

#pragma region �R�}���h�Z�b�g�p�֐�

	/// <summary>
	/// �C���f�b�N�X���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<std::vector<USHORT>>& GetIndices()const { return indices; }
	
	/// <summary>
	/// ���_�o�b�t�@���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<VertexBufferSet>& GetVertexBufferSet()const { return vertexBufferSet; }
	
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̎擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector<IndexBufferSet>& GetIndexBufferSet()const { return indexBufferSet; }
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�̎擾�B
	/// </summary>
	/// <returns></returns>
	ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

#pragma endregion

#pragma region �Z�b�g

#pragma endregion


#pragma region �Q�b�g

	/// <summary>
	/// ���f���t�@�C���Ɋ܂܂�Ă���I�u�W�F�N�g(���f��)�̐����擾���܂��B
	/// </summary>
	/// <returns></returns>
	int GetModelFileObjectNumber()const { return modelFileObjectNum; }

	/// <summary>
	/// �}�e���A�����擾���܂��B
	/// </summary>
	/// <returns></returns>
	Material GetMaterial()const { return material; }


#pragma endregion

};

