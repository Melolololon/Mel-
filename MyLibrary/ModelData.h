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


//���f���̒��_�A�C���f�b�N�X�A�e�N�X�`�����܂Ƃ߂��N���X
class ModelData
{
private:
	static ID3D12Device* device;

	VertexBufferSet vertexBufferSet;
	int modelFileObjectNum = 0;

	//�C���f�b�N�X
	IndexBufferSet indexBufferSet;
	std::vector<USHORT> indices;

	//�e�N�X�`���o�b�t�@�p�q�[�v
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;
	
	//�ǂݎ�����}�e���A��
	Material material;

protected:

	std::vector<Texture>textures;

	/// <summary>
	/// ���_�o�b�t�@�̐������s���܂��B
	/// </summary>
	/// <param name="verticesSize"></param>
	/// <param name="verticesNum"></param>
	void CreateVertexBuffer
	(
		const size_t verticesSize,
		const std::vector<size_t>& verticesNum
	);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�̐����AMap���s���܂��B
	/// </summary>
	/// <param name="indices"></param>
	void CreateIndexBuffer
	(
		const std::vector<std::vector<USHORT>>& indices
	);

	void CteateTextureBuffer();
	void CteateTextureBufferSetColor(const Color& color);

public:
	
	ModelData();
	~ModelData();



};

