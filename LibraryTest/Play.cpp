#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
#include"PrimitiveModel.h"

std::unique_ptr<FbxModel> fbxModel = std::make_unique<FbxModel>();
std::unique_ptr<ObjModel> objModel = std::make_unique<ObjModel>();
std::unique_ptr<PrimitiveModel> priModel = std::make_unique<PrimitiveModel>();

std::unique_ptr<Sprite2D> sprite2D = std::make_unique<Sprite2D>();
std::unique_ptr<Sprite3D> sprite3D = std::make_unique<Sprite3D>();
std::unique_ptr<Texture> tex = std::make_unique<Texture>();


Play::Play(){}


Play::~Play(){}

void Play::Initialize()
{
	sprite2D->CreateSprite();
	sprite3D->CreateSprite({100,100});
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");
	fbxModel->modelNum
	(
		"Resources/cube/cube.fbx",
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

	//sprite2D->Draw(tex.get());

	//sprite3D->SetPosition({ 0,0,0 });
	//sprite3D->SelectDrawAreaDraw({ 64,64 }, {128,128}, tex.get());

	//objModel->Draw(0);

	priModel->Draw(0);
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

