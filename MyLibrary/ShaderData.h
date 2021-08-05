#pragma once

namespace melLib
{
	enum class FORMAT_TYPE
	{
		UNSIGNED_INT,//unsigned int
		SIGNED_INT,//int
		FLOAT,//float
	};

	//パイプライン作成時に
	struct InputLayoutData
	{
		//セマンティクス名
		const char* semantics;

		//送る値の数
		UINT number = 4;

		//送る値の型
		FORMAT_TYPE formatType = FORMAT_TYPE::FLOAT;
	};


	enum class DrawMode
	{
		SOLID,//通常通りモデルを表示します
		WIREFRAME,//ワイヤーフレームで表示します
	};

	enum class CullMode
	{
		NONE,//カリングしません
		FRONT,//表面をカリングします
		BACK//裏面をカリングします
	};

	enum class BlendMode
	{
		NONE,
		ADD,
		SUB,
	};


	//パイプラインの設定をまとめた構造体
	struct PipelineData
	{
		DrawMode drawMode = DrawMode::SOLID;
		CullMode cullMode = CullMode::BACK;
		BlendMode blendMode = BlendMode::ADD;

		bool depthTest = false;
		bool alphaWrite = false;
	};

	//シェーダーの情報をまとめた構造体
	struct ShaderData
	{
		const wchar_t* shaderPath;
		const char* entryPoint;
		const char* ver;
	};
}
