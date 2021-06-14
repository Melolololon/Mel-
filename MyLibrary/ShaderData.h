#pragma once

enum FORMAT_TYPE
{
	FORMAT_TYPE_UNSIGNED_INT,
	FORMAT_TYPE_SIGNED_INT,
	FORMAT_TYPE_FLOAT,
};

//�p�C�v���C���쐬����
struct InputLayoutData
{
	//�Z�}���e�B�N�X��
	const char* semantics;

	//����l�̐�
	UINT number;

	//����l�̌^
	FORMAT_TYPE formatType;
};


enum DrawMode 
{
	DRAW_SOLID,//�ʏ�ʂ胂�f����\�����܂�
	DRAW_WIREFRAME,//���C���[�t���[���ŕ\�����܂�
};

enum CullMode 
{
	CULL_NONE,//�J�����O���܂���
	CULL_FRONT,//�\�ʂ��J�����O���܂�
	CULL_BACK//���ʂ��J�����O���܂�
};

enum BlendMode 
{
	BLEND_NONE,
	BLEND_ADD,
	BLEND_SUB,
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
	DrawMode drawMode = DRAW_SOLID;
	CullMode cullMode = CULL_BACK;
	BlendMode blendMode = BLEND_ADD;
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