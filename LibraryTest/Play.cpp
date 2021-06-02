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

Play::Play(){}


Play::~Play(){}



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
	}
	
	tex = std::make_unique<Texture>();
	spr = std::make_unique<Sprite2D>(Color(255, 0, 0, 120));
	spr->SetScale(Vector2(256, 256));
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");
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
	}
	/*Vector2 sprPos = curve->GetVector2Position();
	spr->SetPosition(sprPos);
	curve->AddNumber(0.02f);*/

	
}

void Play::Draw()
{
	for (int i = 0; i < CREATE_NUM; i++)
	{
		fbxModel->Draw(i);
	}
	spr->Draw();
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

