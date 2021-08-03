#include "Play.h"

#include"CollisionTestObject.h"

#include"GameObjectManager.h"
#include"Camera.h"

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

void Play::Draw()
{
	GameObjectManager::GetInstance()->Draw();

	sprite2DTest->Draw();
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

