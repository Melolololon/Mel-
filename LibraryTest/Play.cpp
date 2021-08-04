#include "Play.h"

#include"CollisionTestObject.h"

#include"GameObjectManager.h"
#include"Camera.h"
#include"SpriteFont2D.h"
#include"TextureFont.h"

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	Camera::Get()->SetRotateCriteriaPosition(Vector3(0, 0, -30));

	GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(0,true));
	GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(Vector3(-10, 0, 0), false));

	Texture::Load("Resources/Texture/testTexture.png", "test");
	sprite2DTest = std::make_unique<Sprite2D>(Texture::Get("test"));
	//sprite2DTest->CreateSetColor(Color(255, 255, 255, 255));
	
	sprite2DTest->SetPosition(Vector2(1280 / 2, 720 / 2));
}

void Play::Update()
{
	if (Input::KeyState(DIK_Q))scale -= 0.05;
	if (Input::KeyState(DIK_E))scale += 0.05;
	sprite2DTest->SetScale(scale);

	GameObjectManager::GetInstance()->Update();


}

Vector2 testScale = 1;
void Play::Draw()
{
	GameObjectManager::GetInstance()->Draw();
	
	
	sprite2DTest->SetDrawArea(0, Vector2(64, 64));
	sprite2DTest->Draw();
	SpriteFont2D::GetInstance()->Draw(0, testScale,SpriteFont2D::CharSequence::BESIDE, "A", TextureFont::Get("testFont"));



	if (Input::KeyState(DIK_Z))testScale.x -= 0.025f;
	if (Input::KeyState(DIK_X))testScale.x += 0.025f;

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

