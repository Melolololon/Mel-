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

	sprite2DTest = std::make_unique<Sprite2D>(Color(255, 255, 255, 255));
	//sprite2DTest->CreateSetColor(Color(255, 255, 255, 255));
	sprite2DTest->SetScale(Vector2(300, 500));

}

void Play::Update()
{
	GameObjectManager::GetInstance()->Update();

	if (Input::KeyState(DIK_SPACE))sprite2DTest->SetColor(Color(255, 0, 0, 255));
	else sprite2DTest->SetColor(Color(255, 255, 255, 255));
}

Vector2 testScale = 1;
void Play::Draw()
{
	GameObjectManager::GetInstance()->Draw();

	SpriteFont2D::GetInstance()->Draw(Vector2(0, 100), testScale,SpriteFont2D::CharSequence::BESIDE, "test", TextureFont::Get("test"));

	//sprite2DTest->Draw();

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

