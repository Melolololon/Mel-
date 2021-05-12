#pragma once
#include"DirectXStruct.h"
#include"Texture.h"
#include"Vector.h"

#include<d3d12.h>
#include<dxgi.h>
#include<d3dx12.h>
#include<vector>
#include<array>

//���_�g���܂킻�����l�������A
//size�Ƃ�uv�Ƃ�float4�ɂ��Ȃ��Ƃ����Ȃ��āA
//�����I�Ɏg�p�����������������疈�񐶐�����
//�g���܂킳�Ȃ���uv��萔�o�b�t�@�Ŏw�肵�Ȃ��Ƃ����Ȃ�����A
//�������ق�����������
//�l�p�`GPU�ɑ����Ă������番�������ق�����������
//�X�v���C�g��������e�b�Z���[�^�ŕ������ăo���o���ɂ����ق�����������


//������p������2D�A3D�X�v���C�g�N���X�����
//Draw�͉��z�֐��B���ꂼ���map����������
//

class Sprite
{
private:

#pragma region �ϐ�
	static const UINT MAX_TEXTURE_LOAD_NUM;
	
	static ID3D12Device* device;
	static ID3D12GraphicsCommandList* cmdList;
	static ComPtr<ID3D12RootSignature>rootSignature;
	static ComPtr<ID3D12PipelineState>defaultPipeline;


#pragma endregion

#pragma region �֐�


#pragma endregion

protected:
	ComPtr<ID3D12PipelineState> pipeline;
	std::array<SpriteVertex, 4> vertices;


#pragma region �o�b�t�@

	//�C���f�b�N�X�́A�X�v���C�g���o���o���ɂ����肷�鏈���Ɏg������������������Ƃ�
	//�W�I���g���ł�邩�������
	VertexBufferSet vertexBufferSet;
	ComPtr<ID3D12Resource> constBuffer;

	static std::vector<ComPtr<ID3D12Resource>> textureBuffer;
	static ComPtr<ID3D12DescriptorHeap> textureHeap;
#pragma endregion

	void CreateBuffer();

	SpriteConstData constData;
#pragma region �֐�


	

	void CommonDataMat();
	void SetCmdList(Texture* texture);

#pragma region �}�b�v
	void MapVertexBuffer(void** data);
	void UnmapVertexBuffer();
#pragma endregion


#pragma endregion

public:
	Sprite();
	~Sprite();

	

	virtual void Draw(Texture* texture);

#pragma region �J���җp�֐�

	static void Initialize(ID3D12Device* dev,ID3D12GraphicsCommandList* cmd);

	static void CreateTextureBuffer(Texture* texture);



#pragma endregion




};

