#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
std::unique_ptr<FbxModel> fbxModel = std::make_unique<FbxModel>();

std::unique_ptr<Sprite2D> sprite2D = std::make_unique<Sprite2D>();
std::unique_ptr<Sprite3D> sprite3D = std::make_unique<Sprite3D>();
std::unique_ptr<Texture> tex = std::make_unique<Texture>();

ModelData data;

sprite spr;
texture sprTex;

Play::Play(){}


Play::~Play(){}

void Play::Initialize()
{
	sprite2D->CreateSprite();
	sprite3D->CreateSprite({100,100});
	tex->LoadSpriteTexture("Resources/Texture/testTexture.png");
	fbxModel->LoadModel
	(
		"Resources/cube/cube.fbx",
		1,
		0
	);

	data.key = "Key";
	Library::CreateBoard({ 10,10 }, data);
	Library::CreateHeapData2({ 255,0,0,255 }, 1, data);

	Library::CreateSprite(&spr);
	sprTex = Library::LoadTexture(L"Resources/Texture/testTexture.png");
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

}

void Play::Draw()
{
	//fbxModel->Draw(0);

	//sprite2D->Draw(tex.get());

	//sprite3D->SetPosition({ 0,0,0 });
    sprite3D->Draw(tex.get());

	//Library::SetPosition({ 120,0,0 }, data, 0);
	//Library::DrawGraphic(data,0);


	//Library::SetIsBillboard(true, true, true);
	//これ出すには、スプライトのシェーダーのヘッダを書き換える
	//Library::DrawSprite3D({ 0,0,0 }, { 100,100 }, spr, sprTex);
}

void Play::end()
{
}

Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

