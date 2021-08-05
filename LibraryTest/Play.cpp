#include "Play.h"

#include"CollisionTestObject.h"

#include"GameObjectManager.h"
#include"Camera.h"
#include"SpriteFont2D.h"
#include"TextureFont.h"
#include"DirectionalLight.h"

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	melLib::Camera::Get()->SetRotateCriteriaPosition(melLib::Vector3(0, 0, -30));

	melLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(0,true));
	melLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(melLib::Vector3(-10, 0, 0), false));

	melLib::Texture::Load("Resources/Texture/testTexture.png", "test");
	sprite2DTest = std::make_unique<melLib::Sprite2D>(melLib::Texture::Get("test"));
	//sprite2DTest->CreateSetColor(Color(255, 255, 255, 255));
	
	sprite2DTest->SetPosition(melLib::Vector2(1280 / 2, 720 / 2));


	melLib::DirectionalLight::Create("test");
	melLib::DirectionalLight::Get("test").SetDirection(melLib::Vector3(-1, 0, 0));
}

void Play::Update()
{
	if (melLib::Input::KeyState(DIK_Q))scale -= 0.05;
	if (melLib::Input::KeyState(DIK_E))scale += 0.05;
	sprite2DTest->SetScale(scale);

	melLib::GameObjectManager::GetInstance()->Update();


}

melLib::Vector2 testScale = 1;
void Play::Draw()
{
	melLib::GameObjectManager::GetInstance()->Draw();
	
	
	sprite2DTest->SetDrawArea(0, melLib::Vector2(64, 64));
	sprite2DTest->Draw();
	melLib::SpriteFont2D::GetInstance()->Draw(0, testScale, melLib::SpriteFont2D::CharSequence::BESIDE, "A", melLib::TextureFont::Get("testFont"));



	if (melLib::Input::KeyState(DIK_Z))testScale.x -= 0.025f;
	if (melLib::Input::KeyState(DIK_X))testScale.x += 0.025f;

}

void Play::Finitialize()
{
}

melLib::Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

