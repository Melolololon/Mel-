#pragma once

enum FORMAT_TYPE
{
	FORMAT_TYPE_UNSIGNED_INT,
	FORMAT_TYPE_SIGNED_INT,
	FORMAT_TYPE_FLOAT,
};

//パイプライン作成時に
struct InputLayoutData
{
	//セマンティクス名
	const char* semantics;

	//送る値の数
	UINT number;

	//送る値の型
	FORMAT_TYPE formatType;
};


enum DrawMode 
{
	DRAW_SOLID,//通常通りモデルを表示します
	DRAW_WIREFRAME,//ワイヤーフレームで表示します
};

enum CullMode 
{
	CULL_NONE,//カリングしません
	CULL_FRONT,//表面をカリングします
	CULL_BACK//裏面をカリングします
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

//パイプラインの設定をまとめた構造体
struct PipelineData
{
	DrawMode drawMode = DRAW_SOLID;
	CullMode cullMode = CULL_BACK;
	BlendMode blendMode = BLEND_ADD;
	DepthMode depthMode = DEPTH_TRUE;
	AlphaWriteMode alphaWriteMode = ALPHA_WRITE_TRUE;
};

//シェーダーの情報をまとめた構造体
struct ShaderData
{
	const wchar_t* shaderPath;
	const char* entryPoint;
	const char* ver;
};