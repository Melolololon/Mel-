#pragma once

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

enum DepthMode 
{
	DEPTH_FALSE,
	DEPTH_TRUE,
};

enum AlphaWriteMode 
{
	ALPHA_WRITE_FALSE,
	ALPHA_WRITE_TRUE,
};

//�p�C�v���C���̐ݒ���܂Ƃ߂��\����
struct PipelineData
{
	DrawMode drawMode = DrawMode::SOLID;
	CullMode cullMode = CullMode::BACK;
	BlendMode blendMode = BlendMode::ADD;
	DepthMode depthMode = DEPTH_TRUE;
	AlphaWriteMode alphaWriteMode = ALPHA_WRITE_TRUE;
};

//�V�F�[�_�[�̏����܂Ƃ߂��\����
struct ShaderData
{
	const wchar_t* shaderPath;
	const char* entryPoint;
	const char* ver;
};