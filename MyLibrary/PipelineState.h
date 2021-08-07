#pragma once
#include"DirectXStruct.h"
#include"ShaderData.h"
#include<string>
#include<unordered_map>
#include<memory>

//�p�C�v���C���͕ʂɃ��f���݂�����static�̃A�N�Z�X�֐��Ƃ�����Ȃ�?
//����Ƃ��A�V�[�����܂����Ŏg���\�����邩�炢��?

namespace MelLib
{
	enum class PipelineStateType
	{
		MODEL,
		SPRITE,
		RENDER_TARGET,
	};
	class PipelineState
	{

	private:
		static std::unordered_map<std::string, std::unique_ptr<PipelineState>>pPipelineState;

		ComPtr<ID3D12PipelineState>pipeline;
		static ID3D12Device* device;

		static void SetPipelineDesc
		(
			const PipelineData& pipelineData,
			D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc,
			const PipelineStateType type,
			const int renderTargetNum
		);


		std::string modelClassName;

		static ID3D12RootSignature* modelRootSignature;
		static ID3D12RootSignature* spriteRootSignature;
		static ID3D12RootSignature* renderTargetRootSignature;


	public:
		PipelineState() {}
		~PipelineState() {}

		static bool Initialize
		(
			ID3D12Device* dev
		);

		/// <summary>
		/// �p�C�v���C���𐶐����܂�
		/// </summary>
		/// <param name="pipelineData">�p�C�v���C�����</param>
		/// <param name="vShaderData">���_�V�F�[�_�[���</param>
		/// <param name="hShaderData">�n���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
		/// <param name="dShaderData">�h���C���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
		/// <param name="gShaderData">�W�I���g���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
		/// <param name="pShaderData">�s�N�Z���V�F�[�_�[���</param>
		/// <param name="pipelineType">���̃p�C�v���C���𐶐����邩</param>
		/// <param name="inputLayoutData">�C���v�b�g���C�A�E�g���(�w�肵�Ȃ��ꍇ�́Anullptr��n��)</param>
		/// <param name="modelClassName">typeid��ModelObject�Ȃǂ�n���Aname�֐��Ŏ擾����������(��Z�b�g�h�~�Ɏg�p)</param>
		/// <param name="renderTargetNum">�����ɂǂ̂��炢�o�͂��邩</param>
		/// <param name="name">�o�^��</param>
		/// <returns></returns>
		static void Create
		(
			const PipelineData& pipelineData,
			const ShaderData& vShaderData,
			const ShaderData& hShaderData,
			const ShaderData& dShaderData,
			const ShaderData& gShaderData,
			const ShaderData& pShaderData,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const std::string& modelClassName,
			const int renderTargetNum,
			const std::string& name
		);

		static PipelineState* Get(const std::string& name) { return pPipelineState[name].get(); }
		static void Delete(const std::string& name);

		/// <summary>
		/// �p�C�v���C���𐶐����܂�
		/// </summary>
		/// <param name="pipelineData">�p�C�v���C�����</param>
		/// <param name="vShaderData">���_�V�F�[�_�[���</param>
		/// <param name="hShaderData">�n���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
		/// <param name="dShaderData">�h���C���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
		/// <param name="gShaderData">�W�I���g���V�F�[�_�[���(�w�肵�Ȃ��ꍇ�́A�p�X��"NULL"���L�q)</param>
		/// <param name="pShaderData">�s�N�Z���V�F�[�_�[���</param>
		/// <param name="pipelineType">���̃p�C�v���C���𐶐����邩</param>
		/// <param name="inputLayoutData">�C���v�b�g���C�A�E�g���(�w�肵�Ȃ��ꍇ�́Anullptr��n��)</param>
		/// <param name="modelClassName">typeid��ModelObject�Ȃǂ�n���Aname�֐��Ŏ擾����������(��Z�b�g�h�~�Ɏg�p)</param>
		/// <param name="renderTargetNum">�����ɂǂ̂��炢�o�͂��邩</param>
		/// <returns></returns>
		bool CreatePipeline
		(
			const PipelineData& pipelineData,
			const ShaderData& vShaderData,
			const ShaderData& hShaderData,
			const ShaderData& dShaderData,
			const ShaderData& gShaderData,
			const ShaderData& pShaderData,
			const PipelineStateType pipelineType,
			const std::vector<InputLayoutData>* inputLayoutData,
			const std::string& modelClassName,
			const int renderTargetNum
		);




		std::string GetModelClassName()
		{
			return modelClassName;
		}

		ComPtr<ID3D12PipelineState> GetPipelineState()
		{
			return pipeline;
		}

		/// <summary>
		/// ���C�u�����Ŏg�p���Ă���p�C�v���C���̏�������data�ɃZ�b�g���܂��B
		/// </summary>
		/// <param name="data"></param>
		/// <param name="type"></param>
		static PipelineData GetDefaultPipelineData( const PipelineStateType type);

		static void SetModelRootSignature(ID3D12RootSignature* sig) { modelRootSignature = sig; }
		static void SetSpriteRootSignature(ID3D12RootSignature* sig) { spriteRootSignature = sig; }
		static void SetRenderTargetRootSignature(ID3D12RootSignature* sig) { renderTargetRootSignature = sig; }

	};
}
