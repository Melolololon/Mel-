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
	fbxModel->LoadModel
	(
		"Resources/boneTest/boneTest.fbx",
		10,
		0
	);


	for (int i = 0; i < 10; i++) 
	{
		Vector3 pos =
		Vector3
		(
			Random::GetRandomNumberRangeSelectFloat(-20,20) ,
			Random::GetRandomNumberRangeSelectFloat(-20,20) ,
			0
		);
		fbxModel->SetPosition(pos, i);

		//アニメーション開始位置セット
		fbxModel->SetCurrentFream(Random::GetRandomNumber(10), i);
		
		//速度倍率セット
		fbxModel->SetAnimationSpeedMagnification(Random::GetRandomNumberRangeSelect(1, 4), i);
	}

	spr = std::make_unique<Sprite3D>();
	tex = std::make_unique<Texture>();
	spr->CreateSprite({8,8});
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");

}
void Play::Update()
{
	for (int i = 0; i < 10; i++) 
		fbxModel->PlayAnimation(i);
}

void Play::Draw()
{
	for(int i = 0; i < 10;i++)
	fbxModel->Draw(i);

	//spr->SelectDrawAreaDraw({ 0,0 }, {64,128}, tex.get());
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

