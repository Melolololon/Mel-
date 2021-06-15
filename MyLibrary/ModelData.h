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
	int modelFileObjectNum = 0;

	//�C���f�b�N�X
	std::vector < IndexBufferSet> indexBufferSet;
	std::vector<std::vector<USHORT>> indices;

	//�e�N�X�`���o�b�t�@�p�q�[�v
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;
	std::vector<Texture*>pTextures;

	//�ǂݎ�����}�e���A��
	Material material;

	void CreateDescriptorHeap(const UINT textureNum);

protected:

	std::vector<Texture>textures;

	/// <summary>
	/// ���_�o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="verticesSize"></param>
	/// <param name="verticesNum"></param>
	void CreateVertexBuffer
	(
		const size_t verticesSize,
		const std::vector<size_t>& verticesNum
	);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�A�r���[�̐����AMap���s���܂��B
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBuffer
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="pTextures"></param>
	void CteateTextureBuffer(const std::vector<Texture*>& pTextures);

	/// <summary>
	/// �e�N�X�`���o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="color"></param>
	void CteateTextureBufferSetColor(const Color& color);

public:
	
	ModelData(){}
	~ModelData(){}

#pragma region �R�}���h�Z�b�g�p�֐�
	const std::vector<std::vector<USHORT>>& GetIndices()const { return indices; }

	const std::vector<VertexBufferSet>& GetVertexBufferSet()const { return vertexBufferSet; }
	const std::vector<IndexBufferSet>& GetIndexBufferSet()const { return indexBufferSet; }
	ID3D12DescriptorHeap* GetTextureDesctiptorHeap()const { return textureDescHeap.Get(); }

#pragma endregion

};

