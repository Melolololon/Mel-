#pragma once

namespace melLib
{
	enum class FORMAT_TYPE
	{
		UNSIGNED_INT,//unsigned int
		SIGNED_INT,//int
		FLOAT,//float
	};

	//�p�C�v���C���쐬����
	struct InputLayoutData
	{
		//�Z�}���e�B�N�X��
		const char* semantics;

		//����l�̐�
		UINT number = 4;

		//����l�̌^
		FORMAT_TYPE formatType = FORMAT_TYPE::FLOAT;
	};


	enum class DrawMode
	{
		SOLID,//�ʏ�ʂ胂�f����\�����܂�
		WIREFRAME,//���C���[�t���[���ŕ\�����܂�
	};

	enum class CullMode
	{
		NONE,//�J�����O���܂���
		FRONT,//�\�ʂ��J�����O���܂�
		BACK//���ʂ��J�����O���܂�
	};

	enum class BlendMode
	{
		NONE,
		ADD,
		SUB,
	};


	//�p�C�v���C���̐ݒ���܂Ƃ߂��\����
	struct PipelineData
	{
		DrawMode drawMode = DrawMode::SOLID;
		CullMode cullMode = CullMode::BACK;
		BlendMode blendMode = BlendMode::ADD;

		bool depthTest = false;
		bool alphaWrite = false;
	};

	//�V�F�[�_�[�̏����܂Ƃ߂��\����
	struct ShaderData
	{
		const wchar_t* shaderPath;
		const char* entryPoint;
		const char* ver;
	};
}
