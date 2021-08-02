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
}

void Play::Update()
{
	GameObjectManager::GetInstance()->Update();
}

void Play::Draw()
{
	GameObjectManager::GetInstance()->Draw();
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

