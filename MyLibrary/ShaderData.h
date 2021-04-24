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

	//���鐔�l�̌^
	FORMAT_TYPE formatType;
};


enum DrawMode 
{
	DRAW_SOLID,
	DRAW_WIREFRAME,
};

enum CullMode 
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK
};

enum BlendMode 
{
	BLEND_NONE,
	BLEND_ADD,
	BLEND_SUB,
};

enum DepthMode 
{
	DEPTH_NONE,
	DEPTH_TRUE,
};

enum AlphaWriteMode 
{
	ALPHA_WRITE_NONE,
	ALPHA_WRITE_TRUE,
};

struct PipelineData
{
	DrawMode drawMode;
	CullMode cullMode;
	BlendMode blendMode;
	DepthMode depthMode;
	AlphaWriteMode alphaWriteMode;
};


struct ShaderData
{
	const wchar_t* shaderPath;
	const char* entryPoint;
	const char* ver;
};