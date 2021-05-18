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

	priModel->CreateBox({ 1,1,0 }, { 255,255,255,255 }, 1);
	//priModel->SetPosition({ 0,0,-10 },0);
	sprite3D->CreateSprite({ 1,1 });
	//sprite3D->SetPosition({ 0,0,-10 });

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
	testTime.SetMaxTime(3);
	testTime.SetStopFlag(false);
}
Vector3 angle = 0;
Vector3 sprPos = 0;
void Play::Update()
{

	if (DirectInput::KeyState(DIK_W))
		sprPos.y += 0.3f;
	if (DirectInput::KeyState(DIK_S))
		sprPos.y -= 0.3f;
	if (DirectInput::KeyState(DIK_A))
		sprPos.x -= 0.3f;
	if (DirectInput::KeyState(DIK_D))
		sprPos.x += 0.3f;
	if (DirectInput::KeyState(DIK_Q))
		sprPos.z -= 0.3f;
	if (DirectInput::KeyState(DIK_E))
		sprPos.z += 0.3f;

	for (int i = 0; i < 10; i++) 
	{
		fbxModel->SetAngle(angle, i);
		fbxModel->PlayAnimation(i);
	}
	if (testTime.GetSameAsMaximumFlag())
		angle += 5.0f;

	sprite3D->SetPosition(sprPos);
	sprite3D->SetScale({ 1,1 });
}

void Play::Draw()
{
	//for(int i = 0; i < 10;i++)
	//fbxModel->Draw(i);

	//sprite2D->Draw(tex.get());

	//if (DirectInput::KeyState(DIK_SPACE))
	//	sprite3D->SetBillboardFlag(true, true, true);
	//else
	//	sprite3D->SetBillboardFlag(false, false, false);

	//sprite3D->SetPosition({ 0,0,0 });

	priModel->Draw(0);
	sprite3D->Draw( tex.get());
	//sprite3D->SelectDrawAreaDraw({ 64,64 }, {128,128}, tex.get());

	//objModel->Draw(0);

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

