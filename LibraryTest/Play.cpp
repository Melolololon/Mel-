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

FreamTimer fTimer(4);

void Play::Initialize()
{
	fbxModel = std::make_unique<FbxModel>();
	objModel = std::make_unique<ObjModel>();
	priModel = std::make_unique<PrimitiveModel>();

	sprite2D = std::make_unique<Sprite2D>();
	sprite3D = std::make_unique<Sprite3D>();
	tex = std::make_unique<Texture>();


	sprite2D->CreateSprite();
	sprite3D->CreateSprite({ 1,1 });
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");
	fbxModel->LoadModel
	(
		"Resources/boneTest/boneTest.fbx",
		10,
		0
	);
	fbxModel->SetPosition({ 3,0,0 }, 1);
	fbxModel->SetPosition({ -3,0,0 }, 2);

	objModel->LoadModel
	(
		"Resources/Obj/testSnake.obj",
		true,
		1,
		0
	);
	objModel->SetPosition({ -10,0,0 }, 0);

	priModel->CreateBox({ 4,4,4 }, { 255,255,255,255 }, 1);

	for (int i = 0; i < 10; i++) 
	{
		fbxModel->SetCurrentFream(Random::GetRandomNumber(10), i);
		
		Vector3 pos =
		{
			Random::GetRandomNumberRangeSelectFloat(-20,20) ,
			Random::GetRandomNumberRangeSelectFloat(-20,20) ,
			0
		};
		fbxModel->SetPosition(pos ,i);
		fbxModel->SetAnimationSpeedMagnification(Random::GetRandomNumberRangeSelect(-3, 3),i);
	}
}
Vector3 angle = 0;
void Play::Update()
{

	if (DirectInput::KeyState(DIK_W))
		angle.y += 3.0f;
	if (DirectInput::KeyState(DIK_S))
		angle.y -= 3.0f;
	if (DirectInput::KeyState(DIK_A))
		angle.x += 3.0f;
	if (DirectInput::KeyState(DIK_D))
		angle.x -= 3.0f;
	fbxModel->SetAngle(angle, 0);

	for (int i = 0; i < 10; i++)
		fbxModel->PlayAnimation(i);
}

void Play::Draw()
{
	for(int i = 0; i < 10;i++)
	fbxModel->Draw(i);

	//sprite2D->Draw(tex.get());

	//if (DirectInput::KeyState(DIK_SPACE))
	//	sprite3D->SetBillboardFlag(true, true, true);
	//else
	//	sprite3D->SetBillboardFlag(false, false, false);

	//sprite3D->SetPosition({ 0,0,0 });
	//sprite3D->SelectDrawAreaDraw({ 64,64 }, {128,128}, tex.get());

	//objModel->Draw(0);

	//priModel->Draw(0);
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

