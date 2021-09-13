#pragma once
#include<d3d12.h>
#include<wrl.h>

#include<string>

#include"Vector.h"
#include"Color.h"

#include"imgui/imgui.h"

//Imgui���Ǘ�����N���X

namespace MelLib 
{

	class ImguiManager
	{
	public:

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


#pragma region ����
		/// <summary>
		/// ���W�I�{�^�����E�B���h�E�ɐ������܂��B
		/// </summary>
		/// <param name="label">���x����(�`�F�b�N�{�b�N�X�ׂ̗ɕ\������镶��)</param>
		/// <param name="pInt">�I������Ă���Ƃ���num�̒l��������ϐ��̃|�C���^</param>
		/// <param name="num">�I������Ă���Ƃ���pInt���Q�Ƃ��Ă���ϐ��ɑ������l</param>
		/// <returns>���݂̃t���[���őI�����ꂽ��true(���m�ɂ́A�I������Ēl�̑���������s��ꂽ��true)</returns>
		bool DrawRadioButton(const std::string& label, int* pInt, const int num);

		/// <summary>
		/// �`�F�b�N�{�b�N�X���E�B���h�E�ɕ\�����܂��B
		/// </summary>
		/// <param name="label">���x����(�`�F�b�N�{�b�N�X�ׂ̗ɕ\������镶��)</param>
		/// <param name="flag">������Ă��邩�ǂ����̃t���O��������ϐ��̃|�C���^</param>
		/// <returns></returns>
		bool DrawCheckBox(const std::string& label, bool* flag);

		bool DrawSliderInt(const std::string& label, int* pInt, const int numMin, const int numMax);
		bool DrawSliderFloat(const std::string& label, float* pFloat, const float numMin, const float numMax);

		bool DrawColorPocker(const std::string& label, Color* pColor,const ImGuiColorEditFlags flag);
#pragma endregion

	};
}

