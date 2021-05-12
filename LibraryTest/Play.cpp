#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
#include"PrimitiveModel.h"


Play::Play(){}


Play::~Play(){}

void Play::Initialize()
{
	fbxModel = std::make_unique<FbxModel>();
	objModel = std::make_unique<ObjModel>();
	priModel = std::make_unique<PrimitiveModel>();

	sprite2D = std::make_unique<Sprite2D>();
	sprite3D = std::make_unique<Sprite3D>();
	tex = std::make_unique<Texture>();


	sprite2D->CreateSprite();
	sprite3D->CreateSprite({1,1});
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");
	fbxModel->modelNum
	(
		"Resources/boneTest/boneTest.fbx",
		1,
		0
	);

	//objModel->LoadModel
	//(
	//	"Resources/Obj/redBall.obj",
	//	true,
	//	1,
	//	0
	//);

	priModel->CreateBox({ 1,1,1 }, { 255,0,0,0 }, 1);



	Library::SetCamera({ 10,0,-10 }, { 0,0,0 }, { 0,1,0 });


	Vector3 a = Vector3(1, 1, 1) + Vector3(3, 2, 1);
	int z = 0;
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



	priModel->SetAngle(angle, 0);
}

void Play::Draw()
{
	//fbxModel->Draw(0);

	sprite2D->Draw(tex.get());

	if (DirectInput::KeyState(DIK_SPACE))
		sprite3D->SetBillboardFlag(true, true, true);
	else
		sprite3D->SetBillboardFlag(false, false, false);

	sprite3D->SetPosition({ 0,0,0 });
	sprite3D->SelectDrawAreaDraw({ 64,64 }, {128,128}, tex.get());

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

