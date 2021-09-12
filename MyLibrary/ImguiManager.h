#pragma once
#include<d3d12.h>
#include<wrl.h>

#include<string>

#include"Vector.h"


#include"imgui/imgui.h"

//Imgui���Ǘ�����N���X

namespace MelLib 
{
	class ImguiManager
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D12DescriptorHeap>desHeap = nullptr;

		bool releaseDrawFlag = true;

		ID3D12Device* pDevice = nullptr;
		ID3D12GraphicsCommandList* pCmdList = nullptr;
	private:
		ImguiManager() {}
		~ImguiManager() {}

		/// <summary>
		/// �����[�X����releaseDrawFlag��false��������true
		/// </summary>
		/// <returns></returns>
		bool CheckReleaseDrawFlag();
	public:
		ImguiManager(ImguiManager& i) = delete;
		ImguiManager& operator=(ImguiManager& i) = delete;
		static ImguiManager* GetInstance();

		bool Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList);
		void Finalize();

		void Begin();
		void Draw();
		
		/// <summary>
		/// �E�B���h�E��`�悷�鏀�����s���܂��B
		/// </summary>
		/// <param name="name">�E�B���h�E��</param>
		/// <param name="pos">���W</param>
		/// <param name="size">�T�C�Y</param>
		void BeginDrawWindow
		(
			const std::string& name,
			const Vector2& pos,
			const Vector2& size
		);

		/// <summary>
		/// �`�揈�����I�����܂��BBeginDrawWindow���Ăяo���A�E�B���h�E�̍��ڂ̐ݒ���s������ɕK���Ăяo���Ă��������B
		/// �����E�B���h�E���쐬����ꍇ�A�e�E�B���h�E�̍��ڂ̐ݒ肪�I�邲�ƂɌĂяo���Ă��������B
		/// </summary>
		void EndDrawWindow();

		/// <summary>
		/// �����[�X���ɃE�B���h�E��`�悷�邩��ݒ肵�܂��B�ݒ肷��ꍇ�́A���C�u�����̏��������O�ŌĂяo���Ă��������B
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseDrawFlag(const bool flag) { releaseDrawFlag = flag; }
	};
}

