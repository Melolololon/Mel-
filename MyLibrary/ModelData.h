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

	//�e�N�X�`���o�b�t�@�p�q�[�v
	ComPtr<ID3D12DescriptorHeap>textureDescHeap;
	std::vector<ComPtr<ID3D12Resource>> textureBuffers;

	//�ǂݎ�����}�e���A��
	Material material;

	//�e�N�X�`�����g��Ȃ��ꍇ�̐F
	Color color;


#pragma region ���_


	template<class VERTEX>
	/// <summary>
	/// ���_�o�b�t�@�A�r���[�̐������s���܂��B
	/// </summary>
	/// <param name="vertexSize"></param>
	/// <param name="vertices"></param>
	void CreateVertexBufferSet
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<VERTEX>>& vertices
	);

	template<class VERTEX>
	void MapVertices(const std::vector<std::vector<VERTEX>>& vertices);

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



protected:

	std::vector<std::unique_ptr<Texture>>pTextures;
	std::vector<std::vector<USHORT>> indices;
	
	/// <summary>
	/// ���_�C���f�b�N�X�o�b�t�@�Z�b�g�̐�����Map���s���܂��B
	/// </summary>
	/// <typeparam name="VERTEX">���_</typeparam>
	/// <param name="vertexSize"></param>
	/// <param name="vertexNum"></param>
	/// <param name="vertices"></param>
	/// <param name="indices"></param>
	template<class VERTEX>
	void BufferPreparationSetTexture
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<VERTEX>>& vertices,
		const std::vector<std::vector<USHORT>>& indices
	);

	template<class VERTEX>
	void BufferPreparationSetColor
	(
		const size_t vertexSize,
		const  std::vector<size_t>& vertexNum,
		const std::vector<std::vector<VERTEX>>& vertices,
		const std::vector<std::vector<USHORT>>& indices
	);

	
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
