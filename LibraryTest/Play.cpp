#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
#include"PrimitiveModel.h"

#include"FreamTimer.h"

#include"Random.h"
#include"RenderTarget.h"

Play::Play(){}


Play::~Play(){}

Vector3 angle = 0;

void Play::Initialize()
{

	fbxModel = std::make_unique<FbxModel>();
	fbxModel->Load
	(
		"Resources/boneTest/boneTest.fbx",
		CREATE_NUM,
		0
	);


	for (int i = 0; i < CREATE_NUM; i++)
	{
		Vector3 pos =
		Vector3
		(
			Random::GetRandomNumberRangeSelect(-20,20) ,
			Random::GetRandomNumberRangeSelect(-20,20) ,
			0
		);
		fbxModel->SetPosition(pos, i);

		//アニメーション開始位置セット
		fbxModel->SetCurrentFream(Random::GetRandomNumber(10), i);
		
		//速度倍率セット
		fbxModel->SetAnimationSpeedMagnification(Random::GetRandomNumberRangeSelect(1, 2), i);

		fbxModel->SetScale(Vector3(4, 4, 4),i);
	}
	
	tex = std::make_unique<Texture>();
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");
	//spr = std::make_unique<Sprite2D>(tex.get());
	spr = std::make_unique<Sprite2D>(tex.get());
	/*spr->SetScale(Vector2(256, 256));*/
	
	//spr->SetDrawArea(Vector2(0,0),Vector2(64,64));
	
	//これ毎回呼び出しているため、毎フレームシーン切り替えすると、消費使用メモリが増え続ける
	


	//std::vector<Vector2>points =
	//{
	//	Vector2(0,0),
	//	Vector2(40,100),
	//	Vector2(500,400),
	//	Vector2(1000,600)
	//};

	//curve = std::make_unique<Curve>();
	//curve->SetPoints(points);



	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_BACK;
	data.depthMode = DEPTH_TRUE;
	data.drawMode = DRAW_SOLID;

	//インプットレイアウト
	std::vector<InputLayoutData> ilData(5);
	ilData[0].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[0].number = 3;
	ilData[0].semantics = "POSITION";
	ilData[1].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[1].number = 2;
	ilData[1].semantics = "TEXCOORD";
	ilData[2].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[2].number = 3;
	ilData[2].semantics = "NORMAL";

	ilData[3].formatType = FORMAT_TYPE::FORMAT_TYPE_UNSIGNED_INT;
	ilData[3].number = 4;
	ilData[3].semantics = "BONEINDICES";
	ilData[4].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[4].number = 4;
	ilData[4].semantics = "BONEWEIGHTS";

	auto result = fbxPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/FbxMultiVertexShader.hlsl","main","vs_5_0" },
		{ L"../MyLibrary/FbxMultiGeometryShader.hlsl","main","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/FbxMultiPixelShader.hlsl","main","ps_5_0" },
		PipelineType::PIPELINE_TYPE_MODEL,
		&ilData,
		typeid(FbxModel).name(),
		2
	);
	fbxModel->SetPipelineAllSet(&fbxPipeline);


	
}

void Play::Update()
{
	
	if (DirectInput::KeyState(DIK_SPACE))
	{
		isEnd = true;
	}

	for (int i = 0; i < CREATE_NUM; i++)
	{
		fbxModel->PlayAnimation(i);
		//fbxModel->SetAngle(angle,i);
	}
	/*Vector2 sprPos = curve->GetVector2Position();
	spr->SetPosition(sprPos);
	curve->AddNumber(0.02f);*/

	if(DirectInput::KeyState(DIK_W))
	{
		angle.x += 2;
	}
	if (DirectInput::KeyState(DIK_S))
	{
		angle.x -= 2;
	}
	if (DirectInput::KeyState(DIK_A))
	{
		angle.y += 2;
	}
	if (DirectInput::KeyState(DIK_D))
	{
		angle.y -= 2;
	}
	Camera::Get()->SetAngle(angle);
}

#include"Camera.h"
void Play::Draw()
{

	for (int i = 0; i < CREATE_NUM; i++)
	{
		fbxModel->Draw(i);
	}
	//spr->Draw();
}

void Play::Finitialize()
{
}

Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

