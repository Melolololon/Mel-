#pragma once
#include<Windows.h>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<random>
#include<ctime>
#include<string>
#include<unordered_map>
#include<wrl.h>


#include"DirectInput.h"
#include"CreateBuffer.h"
#include"CreatePolygon.h"
#include"CreatePipeline.h"
#include"ModelLoader.h"
#include"CreateCamera.h"
#include"Vector.h"
#include"DirectXStruct.h"
#include"DirectXTexLoader.h"

#define DIRECTINPUT_VERSION 0x0800
#include<d3d12.h>
#include<dxgi1_6.h>
#include<d3dx12.h>
#include<DirectXMath.h>
#include<d3dcompiler.h>
#include<dxgidebug.h>
#include<dinput.h>
#include<DirectXTex.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include"Model.h"
#include"PrimitiveModel.h"
#include"ObjModel.h"
#include"FbxModel.h"
#include"Sprite.h"
#include"Sprite2DBase.h"
#include"Sprite3D.h"

//d3d12�̃T���v���ꗗ
//https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/working-samples

enum LibraryPipeline
{
	PIPELINE_NOT_DEPTH_TEST,//�[�x�e�X�g����(z�ɂ�����炸�A�֐��̌Ăяo�������Ԃŕ`�悳��܂�)
	PIPELINE_CULL_NONE,//�J�����O�Ȃ�(���f���̗������`�悵�܂�)
	PIPELINE_NO_WRITE_ALPHA,//�����������������܂Ȃ�
	PIPELINE_NORMAL,
	PIPELINE_OBJ_ANIMATION,
};


using namespace Microsoft::WRL;

class DirectX12 final
{

	enum DrawType
	{
		none,
		box,
		circle,
	};

	enum Dimension
	{
		dimention2D,
		dimention3D
	};

private:
	float clearColor[4];

#pragma region ���݂̕`��R�}���h���
	enum RSState
	{
		RS_MODEL,
		RS_SPRITE,
		RS_POINT,
	};
#pragma endregion


#pragma region Windows
	HWND hwnd;
	int winWidth;
	int winHeight;
#pragma endregion

#pragma region ��{�I�ȃI�u�W�F�N�g

	//DirectX�ϐ�
	HRESULT result;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12Device> dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;


#pragma region RTV

	//���C��
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Resource> backBuffer[2];
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;

#pragma endregion


	ComPtr<ID3D12DescriptorHeap> depthHeap;
	DepthBufferSet depthBufferSet;

	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	D3D12_RESOURCE_BARRIER barrierDesc;


#pragma endregion


	DirectX12();
	~DirectX12();

public:

	DirectX12(DirectX12& d) = delete;
	DirectX12 operator=(DirectX12& d) = delete;
	static DirectX12* GetInstance();

#pragma region �������Ȃǂ̕K�{����

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(HWND hwnd, int windouWidth, int windowHeight);

	/// <summary>
	/// �`��R�}���h�Ȃǂ��Ăяo���O�ɌĂԏ���
	/// </summary>
	void LoopStartProcess();

	//�`�揈���B�R�}���h���X�g�̖��߂����s�����肷��
	void LoopEndProcess();

	void Finalize();
#pragma endregion


	//��ʂ̐F�󂯎��
	void SetScreenColor(Color screenColor);

	//�X�v���C�g�̃T�C�Y�擾
	DirectX::XMFLOAT2 GetTextureSize(int textureHandle);





};

