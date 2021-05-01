#include "PipelineState.h"
#include<d3dcompiler.h>

ID3D12Device* PipelineState::device;
ID3D12RootSignature* PipelineState::modelRootSignature;
ID3D12RootSignature* PipelineState::spriteRootSignature;

PipelineState::PipelineState()
{
	
}

PipelineState::~PipelineState()
{
}



void PipelineState::SetPipelineDesc
(
	const PipelineData& pipelineData,
	D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc
)
{
#pragma region �J�����O�ݒ�


	switch (pipelineData.cullMode)
	{
	case CullMode::CULL_BACK:
		desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;//�J�����O�ݒ�
		break;
	case CullMode::CULL_FRONT:
		desc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;//�J�����O�ݒ�
		break;
	case CullMode::CULL_NONE:
		desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//�J�����O�ݒ�
		break;
	}
	desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

#pragma endregion

#pragma region �h��Ԃ��ݒ�


	switch (pipelineData.drawMode)
	{
	case DrawMode::DRAW_SOLID:
		desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;//�J�����O�ݒ�
		break;
	case DrawMode::DRAW_WIREFRAME:
		desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;//�J�����O�ݒ�
		break;
	}

	//�N���b�s���O�ݒ�
	desc.RasterizerState.DepthClipEnable = true;

#pragma endregion

#pragma region �[�x�ݒ�

	switch (pipelineData.depthMode)
	{
	case DepthMode::DEPTH_NONE:
		desc.DepthStencilState.DepthEnable = false;//
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		break;
	case DepthMode::DEPTH_TRUE:
		desc.DepthStencilState.DepthEnable = true;//
		desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	}
	desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

#pragma endregion

#pragma region �u�����h�ݒ�


	//�u�����h
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};

	blenddesc.BlendEnable = true;//�u�����h��L���ɂ��邩�̃t���O
	switch (pipelineData.blendMode)
	{
	case BlendMode::BLEND_ADD:
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z(�s�����x��100%�ȊO�̎������ƌ��̐F�𑫂�)
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		break;
	case BlendMode::BLEND_SUB:
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_SUBTRACT;//���Z(�s�����x��100%�ȊO�̎������ƌ��̐F�𑫂�)
		blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
		break;
	case BlendMode::BLEND_NONE:
		blenddesc.BlendEnable = false;
		break;
	}
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//�ǂ̐F���u�����h�ł���悤�ɂ��邩�̐ݒ�?ALL����RGBA���u�����h�Ώ�?
	//blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//���Z(�s�����x��100%�ȊO�̎������ƌ��̐F�𑫂�)
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	desc.BlendState.RenderTarget[0] = blenddesc;

#pragma endregion

#pragma region �A���t�@�l�������ݐݒ�


	//�A���t�@��������
	switch (pipelineData.alphaWriteMode)
	{
	case AlphaWriteMode::ALPHA_WRITE_NONE:
		desc.BlendState.AlphaToCoverageEnable = true;
		break;
	case AlphaWriteMode::ALPHA_WRITE_TRUE:
		desc.BlendState.AlphaToCoverageEnable = false;
		break;
	}


#pragma endregion

	//
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;

}



bool PipelineState::CreatePipeline
(
	const PipelineData& pipelineData,
	const ShaderData& vShaderData,
	const ShaderData& gShaderData,
	const ShaderData& hShaderData,
	const ShaderData& dShaderData,
	const ShaderData& pShaderData,
	const PipelineType pipelineType,
	const std::vector<InputLayoutData>* inputLayoutData,
	const std::string& modelClassName
)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc = {};
	SetPipelineDesc(pipelineData, pDesc);
	//���[�g�V�O�l�`���Z�b�g
	switch (pipelineType)
	{
	case PIPELINE_TYPE_MODEL:
		pDesc.pRootSignature = modelRootSignature;
		break;
	case PIPELINE_TYPE_SPRITE:
		pDesc.pRootSignature = spriteRootSignature;
		break;
	}

	HRESULT result;
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> gsBlob;
	ComPtr<ID3DBlob> hsBlob;
	ComPtr<ID3DBlob> dsBlob;
	ComPtr<ID3DBlob>errorBlob;



#pragma region �C���v�b�g���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC>inputLayoutVector;

	if (inputLayoutData)
	{
		inputLayoutVector.reserve(inputLayoutData->size());

		for (const auto& ilData : *inputLayoutData)
		{

			if (ilData.number >= 5)
			{
				OutputDebugStringW(L"inputLayoutData��number��4�𒴂��Ă��܂��B\n");
				return false;
			}

			D3D12_INPUT_ELEMENT_DESC inputLayout;
			DXGI_FORMAT dxgiFormat;

			switch (ilData.formatType)
			{
			case FORMAT_TYPE_FLOAT:
				switch (ilData.number)
				{
				case 1:
					dxgiFormat = DXGI_FORMAT_R32_FLOAT;
					break;
				case 2:
					dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
					break;
				case 3:
					dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
					break;
				case 4:
					dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
					break;
				}
				break;

			case FORMAT_TYPE_UNSIGNED_INT:
				switch (ilData.number)
				{
				case 1:
					dxgiFormat = DXGI_FORMAT_R32_UINT;
					break;
				case 2:
					dxgiFormat = DXGI_FORMAT_R32G32_UINT;
					break;
				case 3:
					dxgiFormat = DXGI_FORMAT_R32G32B32_UINT;
					break;
				case 4:
					dxgiFormat = DXGI_FORMAT_R32G32B32A32_UINT;
					break;
				}
				break;

			case FORMAT_TYPE_SIGNED_INT:
				switch (ilData.number)
				{
				case 1:
					dxgiFormat = DXGI_FORMAT_R32_SINT;
					break;
				case 2:
					dxgiFormat = DXGI_FORMAT_R32G32_SINT;
					break;
				case 3:
					dxgiFormat = DXGI_FORMAT_R32G32B32_SINT;
					break;
				case 4:
					dxgiFormat = DXGI_FORMAT_R32G32B32A32_SINT;
					break;
				}
				break;
			}

			inputLayout =
			{
				ilData.semantics,
				0,
				dxgiFormat,
				0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			};

			inputLayoutVector.push_back(inputLayout);
		}


	}
	if (!inputLayoutData)
	{

		switch (pipelineType)
		{
		case PIPELINE_TYPE_MODEL:
			inputLayoutVector.resize(3);
			inputLayoutVector[0] =
			{
					"POSITION",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					D3D12_APPEND_ALIGNED_ELEMENT,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
			};
			inputLayoutVector[1] =
			{
					"TEXCOORD",
					0,
					DXGI_FORMAT_R32G32_FLOAT,
					0,
					D3D12_APPEND_ALIGNED_ELEMENT,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
			};
			inputLayoutVector[2] =
			{
					"NORMAL",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					D3D12_APPEND_ALIGNED_ELEMENT,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
			};
			break;
		case PIPELINE_TYPE_SPRITE:
			inputLayoutVector.resize(2);
			inputLayoutVector[0] =
			{
					"POSITION",
					0,
					DXGI_FORMAT_R32G32B32_FLOAT,
					0,
					D3D12_APPEND_ALIGNED_ELEMENT,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
			};
			inputLayoutVector[1] =
			{
					"TEXCOORD",
					0,
					DXGI_FORMAT_R32G32_FLOAT,
					0,
					D3D12_APPEND_ALIGNED_ELEMENT,
					D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
					0
			};
			break;
		}
		
	}

	pDesc.InputLayout.pInputElementDescs = inputLayoutVector.data();
	pDesc.InputLayout.NumElements = inputLayoutVector.size();

#pragma endregion

#pragma region �V�F�[�_�[�R���p�C��

	auto GetErrorString = [&errorBlob, &result]()
	{
		if (FAILED(result))
		{
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";

			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	};

#pragma region ���_�V�F�[�_�[

	if (vShaderData.shaderPath == L"LIB")
	{
		result = D3DCompileFromFile(
			L"../MyLibrary/ObjVertexShader.hlsl",
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"VSmain", "vs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&vsBlob, &errorBlob);
	}
	else
	{
		result = D3DCompileFromFile(
			vShaderData.shaderPath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			vShaderData.entryPoint, vShaderData.ver,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&vsBlob, &errorBlob);

	}

	if (result)
	{
		GetErrorString();
		OutputDebugString(L"���_�V�F�[�_�[���ǂݍ��߂܂���ł����B\n");
		return false;
	}

	pDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
#pragma endregion

#pragma region �s�N�Z���V�F�[�_�[

	if (pShaderData.shaderPath == L"LIB")
	{
		result = D3DCompileFromFile(
			L"../MyLibrary/ObjPixelShader.hlsl",
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"PSmain", "ps_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&psBlob, &errorBlob);
	}
	else
	{
		result = D3DCompileFromFile(
			pShaderData.shaderPath,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			pShaderData.entryPoint, pShaderData.ver,
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&psBlob, &errorBlob);
	}

	if (result)
	{
		GetErrorString();
		OutputDebugString(L"�s�N�Z���V�F�[�_�[���ǂݍ��߂܂���ł����B\n");
		return false;
	}

	pDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pDesc.PS.BytecodeLength = psBlob->GetBufferSize();
#pragma endregion

#pragma region �W�I���g���V�F�[�_�[

	if (gShaderData.shaderPath == L"LIB")
	{
		result = D3DCompileFromFile(
			L"../MyLibrary/ObjGeometryShader.hlsl",
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"GSmain", "gs_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&gsBlob, &errorBlob);


	}
	else
		if (gShaderData.shaderPath == L"NULL") {}
		else
		{
			result = D3DCompileFromFile(
				gShaderData.shaderPath,
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				gShaderData.entryPoint, gShaderData.ver,
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0,
				&gsBlob, &errorBlob);

		}

	if (result)
	{
		GetErrorString();
		OutputDebugString(L"�W�I���g���[�V�F�[�_�[���ǂݍ��߂܂���ł����B\n");
		return false;
	}

	if (gShaderData.shaderPath != L"NULL")
	{
		pDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
		pDesc.GS.BytecodeLength = gsBlob->GetBufferSize();
	}
#pragma endregion

#pragma region �n���V�F�[�_�[
	/*result = D3DCompileFromFile(
		L"../MyLibrary/BasicHullShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"HSmain", "hs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&hsBlob, &errorBlob);

	pDesc.HS.pShaderBytecode = hsBlob->GetBufferPointer();
	pDesc.HS.BytecodeLength = hsBlob->GetBufferSize();*/
#pragma endregion

#pragma region �h���C���V�F�[�_�[
	/*	result = D3DCompileFromFile(
			L"../MyLibrary/BasicDomainShader.hlsl",
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"DSmain", "ds_5_0",
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0,
			&dsBlob, &errorBlob);

		pDesc.DS.pShaderBytecode = dsBlob->GetBufferPointer();
		pDesc.DS.BytecodeLength = dsBlob->GetBufferSize();*/
#pragma endregion





#pragma endregion

		//�p�C�v���C���̐���
	result = device->CreateGraphicsPipelineState(&pDesc, IID_PPV_ARGS(&pipeline));
	if (result != S_OK)
	{
		OutputDebugString(L"�p�C�v���C���̐����Ɏ��s���܂���\n");
		return false;
	}

	this->modelClassName = modelClassName;
	return true;
}

bool PipelineState::Initialize
(
	ID3D12Device* dev
)

{
	device = dev;
	return  dev;
}
