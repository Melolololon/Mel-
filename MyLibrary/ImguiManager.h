#pragma once
#include<d3d12.h>
#include<wrl.h>

#include<string>

#include"Vector.h"

//Imgui‚ğŠÇ—‚·‚éƒNƒ‰ƒX

namespace MelLib 
{
	class ImguiManager
	{
	private:
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		ComPtr<ID3D12DescriptorHeap>desHeap = nullptr;

		ID3D12Device* pDevice = nullptr;
		ID3D12GraphicsCommandList* pCmdList = nullptr;
	private:
		ImguiManager() {}
		~ImguiManager() {}
	public:
		ImguiManager(ImguiManager& i) = delete;
		ImguiManager& operator=(ImguiManager& i) = delete;
		static ImguiManager* GetInstance();

		bool Initialize(ID3D12Device* pDev, ID3D12GraphicsCommandList* pCmdList);
		void Finalize();

		void Begin();
		void Draw();
		
		void DrawWindow
		(
			const std::string& name,
			const Vector2& pos,
			const Vector2& size
		);
	};
}

