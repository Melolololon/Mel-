#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#include"DirectXStruct.h"
#include"Color.h"

//�o�b�t�@�[���쐬���邽�߂̃N���X
class CreateBuffer
{
private:
	//����ComPtr�ɂ��邩�m�F����
	ID3D12Device* dev;
	int windowWidth, windowHeight;

	CreateBuffer();
	~CreateBuffer();
public:

	CreateBuffer(CreateBuffer& c) = delete;
	CreateBuffer& operator=(CreateBuffer& c) = delete;
	static CreateBuffer* getInstance();

	void initialize
	(
		ID3D12Device* device,
		const int& windowWidth,
		const int& windowHeight
	);

#pragma region ��

	//����void����Ȃ���bool�ɂ���
	void createVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<Vertex> vertices, VertexBufferSet& set);
	bool createPMDVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<PMDVertex> vertices, PMDVertexBufferSet& set);

	bool createUserVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, VertexBufferSet& set);

	void createIndexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<unsigned short> indices, IndexBufferSet& set);


	void createConstBufferSet(D3D12_HEAP_PROPERTIES cbheapprop, D3D12_RESOURCE_DESC cbresdesc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, void** constData, ConstBufferSet& set,int num);//(num�́A�q�[�v�̗v�f��)
	void createTextureBufferSet(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DirectX::TexMetadata metadata, const DirectX::Image* image, TextureBufferSet& set, int num);
	void createTextureBufferSet2(D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, Color color, TextureBufferSet& set, int num);

	void createDepthBufferSet(D3D12_HEAP_PROPERTIES depthheapprop, D3D12_RESOURCE_DESC debthresdesc, D3D12_CLEAR_VALUE depthclearvalue, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, DepthBufferSet& set);

#pragma region ���[�U�[�쐬
	//void createVertexBufferSet(D3D12_HEAP_PROPERTIES heapprop, D3D12_RESOURCE_DESC resdesc, std::vector<void*> vertices, VertexBufferSet& set);

	//void createUserConstBufferSet(D3D12_HEAP_PROPERTIES cbheapprop, D3D12_RESOURCE_DESC cbresdesc, D3D12_CPU_DESCRIPTOR_HANDLE heapHandle, void* constBuffData, UserConstBufferSet& set);

#pragma endregion

#pragma endregion


#pragma region �V
	/// <summary>
	/// 
	/// </summary>
	/// <param name="heapprop">�v���b�v</param>
	/// <param name="resdesc">�f�X�N</param>
	/// <param name="verticesSize">sizeof�Ōv�Z�����\���̂̃o�C�g��</param>
	/// <param name="verticesNum">���_��</param>
	/// <param name="set">�o�b�t�@�[�Z�b�g</param>
	void createVertexBuffer
	(
		const D3D12_HEAP_PROPERTIES& heapprop,
		const size_t& verticesSize,
		const size_t& verticesNum,
		VertexBufferSet& set
	);

	void createIndexBuffer
	(
		const D3D12_HEAP_PROPERTIES& heapprop, 
		const std::vector<USHORT>& indices, 
		IndexBufferSet& set
	);
#pragma endregion

};

