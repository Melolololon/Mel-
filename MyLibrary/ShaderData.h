#pragma once


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

enum AlphaWrite 
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
	AlphaWrite alphaWrite;
};


struct ShaderData
{
	const wchar_t* shaderPath;
	const char* entryPoint;
	const char* ver;
};